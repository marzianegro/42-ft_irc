#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void	Server::sendToChannel(const std::string &chName, Client *exclude, bool onlyOps) {
	Channel *channel = this->_channels[chName];
	if (!channel) {
		std::cerr << "In sendToChannel, Channel " << chName << " does not exist\n";
		return ;
	}
	
	std::vector<Client*>			regUsers = channel->getRegs();
	std::vector<Client*>::iterator	it_reg = regUsers.begin();
	std::vector<Client*>			opUsers = channel->getOps();
	std::vector<Client*>::iterator	it_op = opUsers.begin();

	for (; it_op != opUsers.end(); it_op++) {
		if (*it_op == exclude)
			continue;
		ftSend((*it_op)->getSocket(), this->_msg);
	}
	for (; !onlyOps && it_reg != regUsers.end(); it_reg++) {
		if (*it_reg == exclude)
			continue;
		ftSend((*it_reg)->getSocket(), this->_msg);
	}
}

// returns null if there is no client with this nick
Client*	Server::findClientByNick(const std::string &nick) {
	std::map<int, Client*>::iterator	it = this->_clients.begin();

	while (it != this->_clients.end()) {
		if (it->second->getNickname() == nick) {
			return (it->second);
		}
		it++;
	}
	return (NULL);
}

std::string trimChannelName(const std::string &channel) {
	std::string	trimmed = channel;

	if (channel[0] == '#' || channel[0] == '&') {
		trimmed = channel.substr(1);
	}

	return (trimmed);
}

bool isChannelValid(const std::string &channel) {
	if (channel.size() < 1 || channel.size() > 50)
		return (false);
	
	return (channel[0] != '#' && channel[0] != '&');
}

bool isNicknameValid(const std::string &nickname) {
	if (nickname.size() < 1 || nickname.size() > 9)
		return (false);
	if (nickname[0] == '#' || nickname[0] == '&' || nickname[0] == '@' || nickname[0] == ':')
		return (false);

	return (nickname.find(' ') == std::string::npos);
}

void ftSend(int fd, std::string &msg) {
	msg += "\r\n";
	std::cout << "SENDING: " << msg << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

std::string fixChannelName(const std::string &channelName) {
	std::string fixed = channelName;
	if (channelName[0] != '#' && channelName[0] != '&')
		fixed = "#" + fixed;
	return (fixed);
}
