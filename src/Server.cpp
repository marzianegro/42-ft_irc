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

void	Server::invite(Client *inviter, Client *invited, const std::string &channel) {
	// REVIEW: is this done in parser? if not, is it correct?
	if (!invited || channel.empty()) {
		this->_msg = errNeedMoreParams(inviter->getNickname(), "INVITE");
	}
	
	std::map<std::string, Channel*>::iterator	it_chan = this->_channels.find(channel);
	std::map<int, Client*>::iterator			it_client = this->_clients.find(invited->getSocket());

	if (it_client == this->_clients.end()) {
		// REVIEW: if invited is null, it means they're not connected (to the server?)
		this->findClientByNick(invited->getNickname());
		this->_msg = "Invited client is not on server";
	}

	if (it_chan == this->_channels.end()) {
		this->_msg = errNoSuchChannel(it_chan->second->getName(), inviter->getNickname());
	} else if (!(it_chan->second->findUser(inviter))) {
		this->_msg = errNotOnChannel(it_chan->first, inviter->getNickname());
	} else if (!it_chan->second->isOperator(inviter)) {
		this->_msg = errChanOPrivsNeeded(channel, inviter->getNickname());
	} else if (it_chan->second->findUser(invited)) {
		this->_msg = errUserOnChannel(it_chan->first, inviter->getNickname(), invited->getNickname());
	} else if (it_chan->second->getCount() >= it_chan->second->getLimit()) {
		this->_msg = errChannelIsFull(channel,invited->getNickname());
	}

	it_chan->second->addInvited(invited);
	this->_msg = rplInviting(channel, inviter->getNickname(), invited->getNickname());
}

void	Server::quit(Client *client, const std::string &reason) {
	std::map<int, Client*>::iterator	it_client = this->_clients.find(client->getSocket());
	
	this->_msg = ":" + client->getNickname() + " QUIT :Quit";
	if (!reason.empty()) {
		this->_msg += ": " + reason;
	}

	std::map<int, Client*>::iterator	it_msg = this->_clients.begin();
	for (; it_msg != this->_clients.end(); it_msg++) {
		send(it_msg->first, this->_msg.c_str(), this->_msg.length(), 0);
	}
	close(it_client->first);
	delete it_client->second;
	this->_clients.erase(it_client);
}

bool	Server::checkPw(const std::string &pw) {
	return (this->_pw == pw);
}

bool	Server::checkNicknames(const std::string &nickname) {
	std::map<int, Client*>::iterator	it = this->_clients.begin();
	for (; it != this->_clients.end(); it++) {
		if (nickname.compare(it->second->getNickname()) == 0) {
			return (false);
		}
	}
	return (true);
}

bool	Server::checkUsernames(const std::string &username) {
	std::map<int, Client*>::iterator	it = this->_clients.begin();
	for (; it != this->_clients.end(); it++) {
		if (username.compare(it->second->getUsername()) == 0) {
			return (false);
		}
	}
	return (true);
}
