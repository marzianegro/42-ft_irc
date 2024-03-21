#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

// TO REMEMBER:
// all msg, topic reason etc have ':' at the beginning

void Server::sendMsgToClient(Client *client, const std::string &target, const std::string &msg) {
	(void)client;
	(void)target; // check if exists
	(void)msg; // send to the client, but only if not empty
}

void Server::sendMsgToChannel(Client *client, const std::string &channel, const std::string &msg, bool onlyOps) {
	(void)client; // check if on the channel
	(void)channel; // check if exists
	(void)msg; // send to all clients on the channel if not empty
	(void)onlyOps; // if true send only to operators
}

void	Server::join(Client *user, std::string &chName, const std::string &key) {
	if (chName[0] == '#') {
		chName = chName.substr(1);
	}

	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(chName);
	this->_msg = "";
	bool chanExist = true;

	if (chName.empty()) {
		std::cout << "1" << std::endl;
		this->_msg = errNeedMoreParams(user->getNickname(), "JOIN");
	} else if (chanIT == this->_channels.end() && key.empty()) {
		std::cout << "2" << std::endl;
		this->_channels[chName] = new Channel(user, chName);
		chanExist = false;
	} else if (chanIT == this->_channels.end()) {
		std::cout << "3" << std::endl;
		this->_channels[chName] = new Channel(user, chName, key);
	} else if (chanIT->second->getKModeStatus() && key != chanIT->second->getKey()) {
		std::cout << "4" << std::endl;
		this->_msg = errBadChannelKey(chName, user->getNickname());
	} else if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
		std::cout << "5" << std::endl;
		this->_msg = errChannelIsFull(chName, user->getNickname());
	} // else if (chanIT->second->getIModeStatus() && !user->getInvitation()) {
		// FIXME: invitation
	// 	this->_msg = errInviteOnlyChan(chName, user->getNickname());
	// }

	std::cout << "server _msg is " << this->_msg << std::endl;

	if (this->_msg.empty()) {
		chanIT = this->_channels.find(chName);
		if (chanExist) {
			chanIT->second->upCount();
			chanIT->second->invitedJoining(user);
			chanIT->second->addUser(user);
		}
		
		// TODO:/FIXME: send to every user on the channel?
		this->_msg = ":" + user->getNickname() + " JOIN #" + chName;
		ftSend(user->getSocket(), this->_msg);
		
		
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

void Server::kick(Client *kicker, Client *kicked, const std::string &chName, const std::string &reason) {
	Channel *channel = this->_channels[chName];
	
	if (chName.empty()) {
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
		channel->downCount();
		this->_msg = ":" + kicker->getNickname() + " KICK #" + chName + kicked->getNickname() + " " + reason;
	}

	if (this->_msg[0] == ':') {
		// TODO: send to channel
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
	}
	
	if (!this->_msg.empty()) {
		ftSend(user->getSocket(), this->_msg);
	}
}

bool checkNick(const std::string &nick) {
	for (size_t i = 0; i < nick.size(); i++) {
		if (!isalnum(nick[i]) && nick[i] != '_' && nick[i] != '-') {
			return (false);
		}
	}
	return (true);
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

