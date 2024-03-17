/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:28:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/16 18:04:07 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

Server::Server() {
}

Server::Server(const Server &src) {
	*this = src;
}

Server::~Server() {
}

Server&	Server::operator=(const Server &src) {
	if (this != &src) {
		this->_port = src._port;
		this->_pw = src._pw;
		this->_serverAddr = src._serverAddr;
		this->_epollFd = src._epollFd;
		this->_event = src._event;
		for (int i = 0; i < MAX_EVENTS; i++) {
			this->_events[i] = src._events[i];
		}
	}
	return (*this);
}

void	Server::setPort(const in_port_t &port) {
	this->_port = port;
}

void	Server::setPw(const std::string &pw) {
	this->_pw = pw;
}

in_port_t	Server::getPort() const {
	return (this->_port);
}

std::string	Server::getPw() const {
	return (this->_pw);
}

void	Server::startServer() {
	this->_serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSock == -1) {
		std::cerr << "An error occured during server socket creation\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Server socket created\n";

	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_port);
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_serverSock, (struct sockaddr*)&this->_serverAddr, sizeof(this->_serverAddr))) {
		std::cerr << std::strerror(errno) << '\n';
		close(this->_serverSock);
		exit(EXIT_FAILURE);
	}
	std::cout << "Server socket bound\n";

	if (listen(this->_serverSock, 128) == -1) {
		std::cerr << std::strerror(errno) << '\n';
		close(this->_serverSock);
		exit(EXIT_FAILURE);
	}
	std::cout << "Server socket now listening\n";
}

void	Server::startEpoll() {
	this->_epollFd = epoll_create1(0);
	if (this->_epollFd == -1) {
		std::cerr << "An error occured during epoll instance creation\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Epoll instance created\n";

	this->_event.events = EPOLLIN;
	this->_event.data.fd = this->_serverSock;

	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, this->_serverSock, &this->_event) == -1) {
		std::cerr << "Failed to add file descriptor to epoll instance\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "File descriptor added to epoll instance\n";
}

std::string	Server::join(Client *user, const std::string &channel, const std::string &key) {

	// joinRequest is the list of channels the user wants to join
	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(channel);
	if (chanIT == this->_channels.end()) {
		return (errNoSuchChannel(channel, NULL)); // there's no inviter here
	}
	if (chanIT->second->getIModeStatus() && !user->getInvitation()) {
		return (errInviteOnlyChan(channel, user->getNickname()));
	}
	if (chanIT->second->getKModeStatus() && key != chanIT->second->getKey()) {
		return (errBadChannelKey(channel, user->getNickname()));
	} else if (!chanIT->second->getKModeStatus() && !key.empty()) {
		// TODO: no key needed for channel
	}
	if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
		return (errChannelIsFull(channel,user->getNickname()));
	}

	chanIT->second->setCount();
	// 1. send msg: "; " + clientName + " is joining the channel " + channelName;
	// 2. channel’s topic (with RPL_TOPIC (332) and no message if channel does not have topic
	chanIT->second->topic(user);
	// 3. list of users currently joined to channel:
	chanIT->second->getOps();
	chanIT->second->getOps();
	// 	- with one or more RPL_NAMREPLY (353) numerics
	// 		- these messages sent by the server MUST include the requesting client that has just joined the channel
	//	- followed by single RPL_ENDOFNAMES (366) numeric)
	return (NULL); // TODO: ???
}

std::string	Server::invite(Client *inviter, Client *invited, const std::string &channel) {
	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(channel);
	
	//TODO: check if invited != NULL, null mean that the invited client is not connected

	if (chanIT == this->_channels.end()) {
		return (errNoSuchChannel(chanIT->second->getName(), inviter->getNickname()));
	}
	if (!(chanIT->second->findUser(inviter))) {
		return (errNotOnChannel(chanIT->first, inviter->getNickname()));
	}
	if (chanIT->second->findUser(invited)) {
		return (errUserOnChannel(chanIT->first, inviter->getNickname(), invited->getNickname()));
	}
	if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
		return (errChannelIsFull(channel,invited->getNickname()));
	}
	invited->setInvitation(true);
	return (NULL); // TODO: ???
}

std::string	Server::quit(Client *client, const std::string &reason) {
	std::map<int, Client*>::iterator	it = this->_clients.find(client->getSocket());
	std::string	msg = ":gerboa QUIT : " + reason + '\n' +
    "                               ; " + client->getNickname() + " is exiting the network with\
                                   the message: " + '"' + reason + '"';

	std::map<int, Client*>::iterator	it_msg = this->_clients.begin();
	while (it_msg != this->_clients.end()) {
		send(it_msg->first, msg.c_str(), msg.length(), 0);
	}
	close(it->first);
	delete it->second;
	this->_clients.erase(it);
	return (NULL); // TODO: ???
}

bool Server::checkPw(const std::string &pw) {
	return (this->_pw == pw);
}
