/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:21:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/15 17:02:28 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include "numReplies.hpp"
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER	512
#define MAX_EVENTS	128

class Channel;
class Client;

class	Server {

public:
	Server(); // ocf default constructor
	Server(const Server &src); // ocf copy constructor
	~Server(); // ocf destructor

	Server&	operator=(const Server &src); // ocf copy assignment operator

	void	setPort(const in_port_t &port);
	void	setPw(const std::string &pw);

	in_port_t	getPort() const;
	std::string	getPw() const;

	void	startServer();
	void	startEpoll();
	void	runEpoll();

	std::string	join(Client *user, std::map<std::string, std::string> &joinRequest);
	std::string	invite(Client *inviter, Client *invited, const std::string &channel);
	std::string	quit(Client *client, const std::string &reason);
	
private:
	in_port_t			_port;
	std::string			_pw;

	int					_serverSock;
	sockaddr_in			_serverAddr;

	int					_epollFd;
	struct epoll_event	_event; // specifying the type of events to monitor
	struct epoll_event	_events[MAX_EVENTS]; // array to hold events returned by epoll_wait()

	std::map<int, Client*>			_clients; // client's fd and object
	std::map<std::string, Channel*>	_channels; // channel's name and object
  
  void newClientConnection();
	void clientEvent(epoll_event &event);
	void execCmd(const std::string &msg, Client *client);
};

