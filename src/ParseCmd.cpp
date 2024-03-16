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
