/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:28:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/12 19:13:19 by mnegro           ###   ########.fr       */
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
		this->_events = src._events;
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
	int	numEvents = epoll_wait(this->_epollFd, this->_event, MAX_EVENTS, 0); // 0 for a non-blocking check
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
			// handle client data
		}
	}
}

/*
	Command: JOIN
	Parameters: <channel>{,<channel>} [<key>{,<key>}]
	Alt Params: 0
*/
void	Server::joinChannel(const Client &client, const std::string &cmd) {
		
}

/*
	Command: INVITE
	Parameters: <nickname> <channel>
*/
int	Channel::invite(Client *client, const std::string &target) {
	if (target channel does not exist) { // look for target channel in server list?
		sendMsg("<client> <channel> :No such channel");
		return (403); // ERR_NOSUCHCHANNEL
	}
	if (client inviting is not on channel) // client not found in this->_clients {
		sendMsg("<client> <channel> :You're not on that channel");
		return (442);
	}
	if (client invited is already on channel) {
		sendMsg("<client> <nick> <channel> :is already on channel");
		return (443);
	} else {
		sendMsg(inviter INVITE invited #channel);
	}
}
