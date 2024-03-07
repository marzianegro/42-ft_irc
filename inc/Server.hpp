/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:21:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/07 11:58:08 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 128

class Channel;
class Client;

class	Server {

public:
	Server(); // ocf default constructor
	Server(const Server &src); // ocf copy constructor
	~Server(); // ocf destructor

	Server&	operator=(const Server &src); // ocf copy assignment operator

	// getters
	in_port_t	getPort();

	// setters
	void	setPort(in_port_t port);
	void	setPw(std::string pw);

	void	startServer();
	void	startEpoll();

private:
	in_port_t			_port;
	std::string			_pw;

	int					_serverSock;
	sockaddr_in			_serverAddr;

	int					_epollFd;
	struct epoll_event	_event; // specifying the type of events to monitor
	struct epoll_event	_events[MAX_EVENTS]; // array to hold events returned by epoll_wait()

	std::vector<Client>				_clients;
	std::map<std::string, Channel>	_channels;
};
