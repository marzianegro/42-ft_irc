/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:00:37 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/16 00:54:41 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Client.hpp"
#include "../incs/Server.hpp"

// FIXME: remember that all msg, topic reason etc have ':' at the beginning

void Server::sendMsgToClient(Client *client, const std::string &target, std::string &msg) {
	std::map<int, Client*>::iterator	clientIT = this->_clients.begin();
	this->_msg = "";

	if (msg.empty()) {
		this->_msg = errNoTextToSend(client->getNickname());
	} else {
		this->_msg = ":" + client->getNickname() + " PRIVMSG " + target + " " + msg;

		for (; clientIT != this->_clients.end(); clientIT++) {
			if (clientIT->second->getNickname() == target) {
				ftSend(clientIT->second->getSocket(), this->_msg);
				return ;
			}
		}
		this->_msg = errNoSuchNick(client->getNickname(), target);
	}
	ftSend(client->getSocket(), this->_msg);
}

void Server::sendMsgToChannel(Client *client, std::string chName, std::string &msg, bool onlyOps) {
	if (chName[0] == '@') {
		chName = chName.substr(1);
	}
	chName = trimChannelName(chName);

	Channel *channel = this->_channels[chName];

	if (msg.empty()) {
		this->_msg = errNoTextToSend(client->getNickname());
		ftSend(client->getSocket(), this->_msg);
	} else if (!channel) {
		this->_msg = errNoSuchChannel(chName, client->getNickname());
		ftSend(client->getSocket(), this->_msg);
	} else if (!channel->findUser(client)) {
		this->_msg = errNotOnChannel(chName, client->getNickname());
		ftSend(client->getSocket(), this->_msg);
	} else {
		this->_msg = ":" + client->getNickname() + " PRIVMSG #" + chName + " " + msg;
		sendToChannel(chName, client, onlyOps);
	}
}

void	Server::join(Client *user, std::string &chName, const std::string &key) {
	std::map<std::string, Channel*>::iterator	it_chan = this->_channels.find(chName);
	this->_msg = "";
	bool chanExist = true;

	if (chName.empty()) {
		this->_msg = errNeedMoreParams(user->getNickname(), "JOIN");
	} else if (it_chan == this->_channels.end() && key.empty()) {
		this->_channels[chName] = new Channel(user, chName);
		chanExist = false;
	} else if (it_chan == this->_channels.end()) {
		this->_channels[chName] = new Channel(user, chName, key);
		chanExist = false;
	} else if (it_chan->second->findUser(user)) {
		this->_msg = errUserOnChannel(chName, user->getNickname(), NULL);
	} else if (it_chan->second->getKModeStatus() && key != it_chan->second->getKey()) {
		this->_msg = errBadChannelKey(chName, user->getNickname());
	} else if (it_chan->second->getCount() >= it_chan->second->getLimit()) {
		this->_msg = errChannelIsFull(chName, user->getNickname());
	} else if (it_chan->second->getIModeStatus() && !it_chan->second->isInvited(user)) {
		this->_msg = errInviteOnlyChan(chName, user->getNickname());
	}

	std::cout << "server _msg is " << this->_msg << std::endl;

	if (this->_msg.empty()) {
		it_chan = this->_channels.find(chName);
		if (chanExist) {
			user->addChannel(chName);
			it_chan->second->upCount();
			it_chan->second->invitedJoining(user);
			it_chan->second->addUser(user);
		}
		
		this->_msg = ":" + user->getNickname() + " JOIN #" + chName;
		sendToChannel(chName, NULL, false);
		
		this->_msg = it_chan->second->getTopic(user);
		ftSend(user->getSocket(), this->_msg);

		this->_msg = rplNamReply(*it_chan->second, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
		this->_msg = rplEndOfNames(it_chan->first, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
	} else {
		ftSend(user->getSocket(), this->_msg);
	}
}

void	Server::part(Client *user, std::string &chName, std::string reason) {
	Channel *channel = this->_channels[chName];

	if (reason.empty()) {
		reason = "Leaving";
	}

	if (chName.empty()) {
		this->_msg = errNeedMoreParams(user->getNickname(), "PART");
	}	else if (!channel) {
		this->_msg = errNoSuchChannel(chName, user->getNickname());
	} else if (!channel->findUser(user)) {
		this->_msg = errNotOnChannel(chName, user->getNickname());
	}

	if (this->_msg.empty()) {
		this->_msg = ":" + user->getNickname() + " PART #" + chName + " " + reason;
		sendToChannel(chName, NULL, false);
		channel->downCount();
		channel->removeUser(user);
		this->checkChOperators(channel);
		user->removeChannel(chName);
	}
}

void Server::kick(Client *kicker, Client *kicked, std::string &chName, const std::string &reason) {
	bool hasBeenKicked = false;

	std::cout << "channel name is " << chName << std::endl;
	Channel *channel = this->_channels[chName];
	
	if (chName.empty() || !kicked) {
		this->_msg = errNeedMoreParams(kicker->getNickname(), "KICK");
	} else if (!channel) {
		this->_msg = errNoSuchChannel(chName, kicker->getNickname());
	} else if (!channel->findUser(kicker)) {
		this->_msg = errNotOnChannel(chName, kicker->getNickname());
	} else if (!channel->isOperator(kicker)) {
		this->_msg = errChanOPrivsNeeded(chName, kicker->getNickname());
	} else if (!kicked || !channel->removeUser(kicked)) { //FIXME: siamo sicuri??? forse meglio un "isUserInChannel"
		this->_msg = errUserNotInChannel(chName, kicker->getNickname(), kicked->getNickname());
	} else {
		hasBeenKicked = true;
		channel->removeUser(kicked);
		this->checkChOperators(channel);
		channel->downCount();
		kicked->removeChannel(chName);
		this->_msg = ":" + kicker->getNickname() + " KICK #" + chName + " " + kicked->getNickname() + " " + reason;
	}

	if (hasBeenKicked) {
		ftSend(kicked->getSocket(), this->_msg);
		sendToChannel(chName, NULL, false);
	} else {
		ftSend(kicker->getSocket(), this->_msg);
	}
}

void Server::topic(Client *user, const std::string &chName, const std::string &topic) {
	this->_msg = "";
	Channel *channel = this->_channels[chName];
	
	if (chName.empty()) {
		this->_msg = errNeedMoreParams(user->getNickname(), "TOPIC");
	} else if (!channel) {
		this->_msg = errNoSuchChannel(chName, user->getNickname());
	} else if (!channel->findUser(user)) {
		this->_msg = errNotOnChannel(chName, user->getNickname());
	} else if (channel->getTModeStatus() && !channel->isOperator(user)) {
		this->_msg = errChanOPrivsNeeded(chName, user->getNickname());
	} else if (topic.empty()) {
		this->_msg = channel->getTopic(user);
	} else {
		channel->setTopic(topic);
		this->_msg = ":" + user->getNickname() + " TOPIC #" + chName + " " + topic;
		sendToChannel(chName, NULL, false);
		return ;
	}
	
	if (!this->_msg.empty()) {
		ftSend(user->getSocket(), this->_msg);
	}
}
void Server::mode(Client *user, const std::string &chName, const std::vector<std::string> &mode) {
	Channel *channel = this->_channels[chName];

	if (!channel) {
		this->_msg = errNoSuchChannel(chName, user->getNickname());
	} else if (!channel->findUser(user)) {
		this->_msg = errNotOnChannel(chName, user->getNickname());
	} else if (mode.empty()) {
		this->_msg = rplChannelModeIs(chName, user->getNickname(), channel);
	} else if (!channel->isOperator(user)) {
		this->_msg = errChanOPrivsNeeded(chName, user->getNickname());
	} else {
		std::vector<std::string>::const_iterator it_mode = mode.begin();
		while (it_mode != mode.end()) {
			if (it_mode->length() == 2 && (*it_mode)[0] == '+') {
				modeSet(mode, channel, user);
			} else if (it_mode->length() == 2 && (*it_mode)[0] == '-') {
				modeUnset(mode, channel, user);
			}
			++it_mode;
		}
		// this->_msg = rplChannelModeIs(chName, user->getNickname(), channel);
		// sendToChannel(chName, NULL, false);
		this->_msg = "";
	}

	if (!this->_msg.empty()) {
		ftSend(user->getSocket(), this->_msg);
	}	
}

void Server::nick(Client *client, const std::string &newNick) {
	this->_msg = "";

	if (newNick.empty()) {
		this->_msg = errNoNicknameGiven("no-name");
	} else if (!checkNick(newNick)) {
		this->_msg = errErroneusNickname("no-name", newNick);
	} else if (!checkNicknames(newNick)) {
		this->_msg = errNicknameInUse("no-name", newNick);
	}
	
	if (this->_msg.empty()) {
		if (!client->getNickname().empty()) {
			this->_msg = ":" + client->getNickname() + " NICK " + newNick;
			this->sendToEveryone(NULL);
		} else {
			this->_msg = ":" + newNick + " NICK " + newNick;
			ftSend(client->getSocket(), this->_msg);
			this->_msg = rplWelcome(newNick);
		}

		client->setNickname(newNick);
	}
	ftSend(client->getSocket(), this->_msg);
}

void Server::user(Client *client, const std::string &username, const std::string &realname) {
	this->_msg = "";

	if (username.empty() || realname.empty()) {
		this->_msg = errNeedMoreParams(client->getNickname(), "USER");
	} else if (!client->getUsername().empty()) {
		this->_msg = errAlreadyRegistered(client->getNickname());
	}

	if (this->_msg.empty()) {
		client->setUsername(username);
		client->setRealname(realname);
	} else {
		ftSend(client->getSocket(), this->_msg);
	}
}

void Server::ping(Client *client, const std::string &token) {
	if (token.empty()) {
		this->_msg = errNoOrigin(client->getNickname());
	} else {
		this->_msg = "PONG gerboa " + token;
	}

	ftSend(client->getSocket(), this->_msg);
}

void Server::pong(Client *client) {
	std::cout << "PONG received by" << client->getNickname() << '\n';
}

void	Server::who(Client *user, const std::string &chName) {
	Channel *channel = this->_channels[chName];

 	if (!channel) {
		this->_msg = errNoSuchChannel(chName, user->getNickname());
	} else if (!channel->findUser(user)) {
		this->_msg = errNotOnChannel(chName, user->getNickname());
	}

	if (this->_msg.empty()) {
		this->_msg = rplNamReply(*channel, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
		this->_msg = rplEndOfNames(chName, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
	} else {
		ftSend(user->getSocket(), this->_msg);
	}
}
