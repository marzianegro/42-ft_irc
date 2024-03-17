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

void Server::kick(Client *kicker, Client *kicked, const std::string &channel, const std::string &reason) {
	(void)kicker;
	(void)kicked; // if NULL, does not exist
	(void)channel; // check if exists
	(void)reason; // can be empty
}

void Server::topic(Client *user, const std::string &channel, const std::string &topic) {
	(void)user; // need permission and have to be on the channel
	(void)channel; // check if exists
	(void)topic; // if empty, send topic back, if only ":" clear topic, else set topic
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

