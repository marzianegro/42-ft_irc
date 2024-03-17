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
