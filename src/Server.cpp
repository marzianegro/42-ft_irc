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

void	Server::runEpoll() {
	int	numEvents = epoll_wait(this->_epollFd, &this->_event, MAX_EVENTS, 0); // 0 for a non-blocking check
	if (numEvents == -1) {
		std::cerr << "epoll_wait() failed\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < numEvents; i++) {
		if (this->_events[i].data.fd == this->_serverSock) {
			int	clientSock = accept(this->_serverSock, NULL, NULL);
			if (clientSock == -1) {
				std::cerr << std::strerror(errno) << '\n';
				continue ;
			}
			if (fcntl(clientSock, F_SETFL, O_NONBLOCK) == -1) {
				std::cerr << "fcntl() failed to set client socket to non-blocking\n";
				close(clientSock);
				continue ;
			}
			std::cout << "Client socket accepted\n";
			this->_event.data.fd = clientSock;
			if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, clientSock, &this->_event) == -1) {
				std::cerr << "Failed to add file descriptor to epoll instance\n";
				exit(EXIT_FAILURE);
			}
			std::cout << "File descriptor added to epoll instance\n";
		} else {
			int	clientSock = this->_events[i].data.fd;
			// handling client data
			this->_clients[clientSock] = new Client(clientSock);
			// TODO: @skyheis parse input
		}
	}
}

std::string	Server::join(Client *user, std::map<std::string, std::string> &joinRequest) {

	// joinRequest is the list of channels the user wants to join
	std::map<std::string, std::string>::iterator reqIT = joinRequest.begin();
	std::map<std::string, Channel*>::iterator	chanIT = this->_channels.find(reqIT->first);
	for (; reqIT != joinRequest.end(); reqIT++) {
		if (chanIT == this->_channels.end()) {
			return (errNoSuchChannel(joinRequest[reqIT->first], NULL)); // there's no inviter here
		}
		if (chanIT->second->getIModeStatus() && !user->getInvitation()) {
			return (errInviteOnlyChan(joinRequest[reqIT->first], user->getNickname()));
		}
		if (chanIT->second->getKModeStatus() && reqIT->second != chanIT->second->getKey()) {
			return (errBadChannelKey(joinRequest[reqIT->first], user->getNickname()));
		} else if (!chanIT->second->getKModeStatus() && !reqIT->second.empty()) {
			// TODO: no key needed for channel
		}
		if (chanIT->second->getCount() >= chanIT->second->getLimit()) {
			return (errChannelIsFull(joinRequest[reqIT->first],user->getNickname()));
		}
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
