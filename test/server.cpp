/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/07/01 19:15:24 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include "Server.hpp"

int sock_fd = 0;
int	server_life = 1;

int	main(int ac, char **av) {
	t_server	srv;

	if (ac != 3) {
		std::cerr << "Please enter provide the folling argument:\n\t\
		Port [1024-65535]\n\tPassword [3+ chars]\n\n\
		ex: ./server 6642 foo" << std::endl;	
		return (1);
	}
	
	init_server(av[1], av[2], srv);

	std::cout << "Starting server life...\n" << std::endl;
	ft_server_life(srv);
	
	close(sock_fd);
	return (0);
}

