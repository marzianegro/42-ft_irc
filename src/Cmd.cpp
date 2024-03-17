#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::sendToClient(Client *client, const std::string &target, const std::string &msg) {
	(void)client;
	(void)target;
	(void)msg;
}

void Server::sendToChannel(Client *client, const std::string &channel, const std::string &msg, bool onlyOps) {
	(void)client;
	(void)channel;
	(void)msg;
	(void)onlyOps;
}
