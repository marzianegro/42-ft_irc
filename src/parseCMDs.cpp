#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::parseJoin(Client *client, std::string msg) {
	std::cout << "CALLING JOIN: " << msg << std::endl;
	std::istringstream ssmsg(msg);
	std::string channels, keys, channel, key;
	std::getline(ssmsg, channels, ' ');
	std::getline(ssmsg, keys, ' ');
	std::istringstream ssch(channels), ssky(keys);

	while (std::getline(ssch, channel, ',')) {
		if (!std::getline(ssky, key, ','))
			key = "";
		std::cout << "JOINING: " << channel << " WITH KEY: " << key << std::endl;
		this->join(client, channel, key);
	}
}

bool isPrivmsgToChannel(const std::string &channel) {
	return channel[0] == '#' || channel[0] == '&' || channel[0] == '@';
}

bool onlyOps(const std::string &channel) {
	std::size_t pos = channel.find_first_of('@');
	return pos != std::string::npos && channel.size() > 2 && pos < 2;
}

void Server::parsePrivmsg(Client *client, std::string msg) {
	std::istringstream ssmsg(msg);
	std::string target, message;
	std::getline(ssmsg, target, ' ');
	std::getline(ssmsg, message);

	if (isPrivmsgToChannel(target)) {
		this->sendMsgToChannel(client, target, message, onlyOps(target));
	} else {
		this->sendMsgToClient(client, target, message);
	}
}

void Server::parseInvite(Client *client, std::string msg) {
	Client *invited = NULL;
	std::istringstream ssmsg(msg);
	std::string nickname, channel;
	std::getline(ssmsg, nickname, ' ');
	std::getline(ssmsg, channel);

	invited = this->findClientByNick(nickname);
	
	if (!isChannelValid(channel)) {
		this->_msg = errNoSuchChannel(channel, client->getNickname());
		ftSend(client->getSocket(), this->_msg);
	} else {
		channel = trimChannelName(channel);
		this->invite(client, invited, channel);
	}
}

void Server::parseKick(Client *client, std::string msg) {
	std::istringstream ssmsg(msg);
	std::string chName, nickname, reason;
	std::getline(ssmsg, chName, ' ');
	std::getline(ssmsg, nickname, ' ');
	std::getline(ssmsg, reason);

	if (!isChannelValid(chName)) {
		this->_msg = errNoSuchChannel(chName, client->getNickname());
		ftSend(client->getSocket(), this->_msg);
		return ;
	}

	chName = trimChannelName(chName);

	Client *kicked = this->findClientByNick(nickname);

	this->kick(client, kicked, chName, reason);
}

void Server::parseTopic(Client *client, std::string msg) {
	std::istringstream ssmsg(msg);
	std::string channel, topic;
	std::getline(ssmsg, channel, ' ');
	std::getline(ssmsg, topic);

	channel = trimChannelName(channel);

	this->topic(client, channel, topic);
}

void Server::parseMode(Client *client, std::string msg) {
	(void)client;
	(void)msg;
	// TODO:
}

void Server::parseQuit(Client *client, std::string msg) {
	if (msg.empty()) {
		msg = "Client quit";
	this->quit(client, msg);
	}
}

void Server::parseUser(Client *client, std::string msg) {
	std::istringstream ssmsg(msg);
	std::string username, realname, tmp;
	std::getline(ssmsg, username, ' ');
	std::getline(ssmsg, tmp, ' ');
	std::getline(ssmsg, tmp, ' ');
	std::getline(ssmsg, realname);

	this->user(client, username, realname);
}

void Server::parseOper(Client *client, std::string msg) {
	(void)client;
	(void)msg;
}
