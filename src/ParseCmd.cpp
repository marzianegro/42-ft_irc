#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::parseJoin(Client *client, std::string msg) {
	std::istringstream ssmsg(msg);
	std::string channels, keys, channel, key;
	std::getline(ssmsg, channels, ' ');
	std::getline(ssmsg, keys, ' ');
	std::istringstream ssch(channels), ssky(keys);

	while (std::getline(ssch, channel, ',')) {
		if (!std::getline(ssky, key, ','))
			key = "";
		this->join(client, channel, key);
	}
}

bool privmsgToChannel(const std::string &channel) {
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

	if (privmsgToChannel(target)) {
		this->sendToChannel(client, target, message, onlyOps(target));
	} else {
		this->sendToClient(client, target, message);
	}
}

void Server::parseInvite(Client *client, std::string msg) {
	Client *invited = NULL;
	std::istringstream ssmsg(msg);
	std::string nickname, channel;
	std::getline(ssmsg, nickname, ' ');
	std::getline(ssmsg, channel);

	invited = this->findClientByNick(nickname);
	this->invite(client, invited, channel);
}
