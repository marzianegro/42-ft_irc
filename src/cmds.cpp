/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:00:37 by mnegro            #+#    #+#             */
/*   Updated: 2024/04/09 09:00:41 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

// TO REMEMBER:
// all msg, topic reason etc have ':' at the beginning

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

void Server::sendMsgToChannel(Client *client, std::string &chName, std::string &msg, bool onlyOps) {
	if (chName[0] == '@') {
		chName = chName.substr(1);
	}
	if (chName[0] == '#') {
		chName = chName.substr(1);
	}
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
	if (chName[0] == '#') {
		chName = chName.substr(1);
	}

	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(chName);
	this->_msg = "";
	bool chanExist = true;

	if (chName.empty()) {
		this->_msg = errNeedMoreParams(user->getNickname(), "JOIN");
	} else if (chanIT == this->_channels.end() && key.empty()) {
		this->_channels[chName] = new Channel(user, chName);
		chanExist = false;
	} else if (chanIT == this->_channels.end()) {
		this->_channels[chName] = new Channel(user, chName, key);
	} else if (chanIT->second->getKModeStatus() && key != chanIT->second->getKey()) {
		this->_msg = errBadChannelKey(chName, user->getNickname());
	} else if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
		this->_msg = errChannelIsFull(chName, user->getNickname());
	} else if (chanIT->second->getIModeStatus() && !chanIT->second->isInvited(user)) {
		this->_msg = errInviteOnlyChan(chName, user->getNickname());
	}

	std::cout << "server _msg is " << this->_msg << std::endl;

	if (this->_msg.empty()) {
		chanIT = this->_channels.find(chName);
		if (chanExist) {
			user->addChannel(chName);
			chanIT->second->upCount();
			chanIT->second->invitedJoining(user);
			chanIT->second->addUser(user);
		}
		
		this->_msg = ":" + user->getNickname() + " JOIN #" + chName;
		sendToChannel(chName, NULL, false);
		
		this->_msg = chanIT->second->getTopic(user);
		ftSend(user->getSocket(), this->_msg);

		this->_msg = rplNamReply(*chanIT->second, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
		this->_msg = rplEndOfNames(chanIT->first, user->getNickname());
		ftSend(user->getSocket(), this->_msg);
	} else {
		ftSend(user->getSocket(), this->_msg);
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
	} else if (!kicked || !channel->removeUser(kicked)) {
		this->_msg = errUserNotInChannel(chName, kicker->getNickname(), kicked->getNickname());
	} else {
		hasBeenKicked = true;
		channel->downCount();
		kicked->removeChannel(chName);
		this->_msg = ":" + kicker->getNickname() + " KICK #" + chName + kicked->getNickname() + " " + reason;
	}

	if (hasBeenKicked) {
		sendToChannel(chName, NULL, false); // REVIEW: kicker escluso?
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
		this->_msg = rplTopic(channel->getName(), user->getNickname(), topic);
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
	} else if (!channel->isOperator(user)) {
		this->_msg = errChanOPrivsNeeded(chName, user->getNickname());
	}
	
	std::vector<std::string>::const_iterator it_mode = mode.begin();
	while (it_mode != mode.end()) {
		if (it_mode->length() == 2 && (*it_mode)[0] == '+') {
			modeSet(mode, channel, user);
		} else if (it_mode->length() == 2 && (*it_mode)[0] == '-') {
			modeUnset(mode, channel, user);
		}
	}

	this->_msg = rplChannelModeIs(chName, user->getNickname(), channel);
}

void Server::nick(Client *client, const std::string &newNick) {
	this->_msg = "";

	std::cout << "newNick is " << newNick << std::endl;

	if (newNick.empty()) {
		this->_msg = errNoNicknameGiven("no-name");
	} else if (!checkNick(newNick)) {
		this->_msg = errErroneusNickname("no-name", newNick);
	} else if (!checkNicknames(newNick)) {
		this->_msg = errNicknameInUse("no-name", newNick);
	}
	
	if (this->_msg.empty()) {
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
