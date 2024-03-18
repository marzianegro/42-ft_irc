#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

// return null if there is no client with this nick
Client *Server::findClientByNick(const std::string &nick) {
	std::map<int, Client*>::iterator	it = this->_clients.begin();
	std::map<int, Client*>::iterator	ite = this->_clients.end();

	while (it != ite) {
		if (it->second->getNickname() == nick)
			return (it->second);
		it++;
	}
	return (NULL);
}

std::string trimChannelName(const std::string &channel) {
	std::string	trimmed = channel;

	if (channel[0] == '#' || channel[0] == '&')
		trimmed = channel.substr(1);

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
	send(fd, msg.c_str(), msg.size(), 0);
}

std::string fixChannelName(const std::string &channelName) {
	std::string fixed = channelName;
	if (channelName[0] != '#' && channelName[0] != '&')
		fixed = "#" + fixed;
	return (fixed);
}
