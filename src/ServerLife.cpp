/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:28:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/13 19:26:25 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::newClientConnection() {
	int	clientSock = accept(this->_serverSock, NULL, NULL);
	if (clientSock == -1) {
		std::cerr << std::strerror(errno) << '\n';
		return ;
	}

	if (fcntl(clientSock, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl() failed to set client socket to non-blocking\n";
		close(clientSock);
		return ;
	}
	std::cout << "Client socket accepted\n";

	this->_event.data.fd = clientSock;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, clientSock, &this->_event) == -1) {
		std::cerr << "Failed to add file descriptor to epoll instance\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "File descriptor added to epoll instance\n";
	this->_clients[clientSock] = new Client(clientSock);
}

void Server::clientEvent(epoll_event &event) {
	char buffer[MAX_BUFFER];
	bzero(buffer, sizeof(buffer));

	ssize_t byteRecv = recv(event.data.fd, buffer, sizeof(buffer), 0);
	if (byteRecv == 0) {
		std::cout << "Connection closed by the client\n";
		epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, event.data.fd, &event);
	} else if (byteRecv == -1) {
		std::cerr << "Error while receiving the message\n";
	} else {
		// handle string
		std::string msg;
		std::map<int, Client*>::iterator	it = this->_clients.find(event.data.fd);
		
		it->second->fillBuffer(buffer);
		msg = it->second->readBuffer();
		while (!msg.empty()) {
			this->execCmd(msg, it->second);
			msg = it->second->readBuffer();
		}
	}
}

void	Server::runEpoll() {
	int numEvents = epoll_wait(this->_epollFd, this->_events, MAX_EVENTS, 0); // 0 for a non-blocking check
	if (numEvents == -1) {
		std::cerr << "epoll_wait() failed\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < numEvents; i++) {
		if (this->_events[i].data.fd == this->_serverSock) { // new client connection
			this->newClientConnection();
		} else {
			this->clientEvent(this->_events[i]);
		}
	}
}

void Server::execCmd(const std::string &msg, Client *client) {
	std::string possibleCmd[] = {"JOIN", "INVITE"};
	int			lenght = sizeof(possibleCmd);
	std::string cmd;
	std::size_t	pos = msg.find(' ');

	cmd = msg.substr(0, pos);

	int cmdPos = 0;
	while (cmdPos < lenght) {
		if (cmd == possibleCmd[cmdPos]) {
			// call function
			return ;
		}
		cmdPos++;
	}

	switch (cmdPos) {
		case 0:
			break;

		case 1:
			break;

		default:
			// error
			// CMD not found
			break;
	}
	
	
}
