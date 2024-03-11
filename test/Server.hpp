#pragma once

#include <iostream>
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>

#define MAX_BUF		512
#define MAX_EVENTS	1024

extern int sock_fd;
extern int	server_life;

typedef struct	s_server {
	int			socket;
	in_port_t	port;
	sockaddr_in	addr;
	std::string	passwd;
	int			poll_fd;
	epoll_event	ev_def;
	epoll_event ev_lst[MAX_EVENTS];
	int			ev_nums;
	int			client_fd;
	sockaddr_in client_addr;
	socklen_t	client_addr_len;

	// std::map<int, Client*>			client_map;
	// std::map<std::string, Channel>	channels;
	std::vector<std::string>		nicknames;

	char		buffer[MAX_BUF];
	ssize_t 	bytes_read;
}				t_server;


void	init_server(std::string const &port, std::string const &passwd, t_server &srv);
void	ft_server_life(t_server &srv);
