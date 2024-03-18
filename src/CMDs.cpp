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

void	Server::join(Client *user, const std::string &chName, const std::string &key) {
	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(chName);
	this->_msg = "";

	if (chName.empty()) {
		this->_msg = errNeedMoreParams(user->getNickname(), "JOIN");
	} else if (chanIT == this->_channels.end() && key.empty()) {
		this->_channels[chName] = new Channel(user, chName);
	} else if (chanIT == this->_channels.end()) {
		this->_channels[chName] = new Channel(user, chName, key);
	} else if (chanIT->second->getKModeStatus() && key != chanIT->second->getKey()) {
		this->_msg = errBadChannelKey(chName, user->getNickname());
	} else if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
		this->_msg = errChannelIsFull(chName, user->getNickname());
	} else if (chanIT->second->getIModeStatus() && !user->getInvitation()) {
		this->_msg = errInviteOnlyChan(chName, user->getNickname());
	}

	if (this->_msg.empty()) {
		chanIT = this->_channels.find(chName);
		chanIT->second->upCount();
		chanIT->second->invitedJoining(user);
		chanIT->second->addUser(user);
		
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
		channel->setTopic(user, topic);
	}
	
	if (!this->_msg.empty()) {
		ftSend(user->getSocket(), this->_msg);
	}
}

void Server::nick(Client *client, const std::string &newNick) {
	(void)client;
	(void)newNick; // check if empty
	// ERR_NONICKNAMEGIVEN (431)
	// ERR_ERRONEUSNICKNAME (432)
	// ERR_NICKNAMEINUSE (433)
	// ERR_NICKCOLLISION (436)
}

void Server::user(Client *client, const std::string &username, const std::string &realname) {
	
	(void)client;
	(void)username;
	(void)realname;
	// ERR_NEEDMOREPARAMS (461)
	// ERR_ALREADYREGISTRED (462)
}

void Server::ping(Client *client, const std::string &token) {
	(void)client;
	(void)token; // this must not be empty
	// PONG: send a pong msg to the client
	// example: PONG gerboa <token>
}

void Server::pong(Client *client) {
	std::cout << "PONG received by" << client->getNickname() << '\n';
}

