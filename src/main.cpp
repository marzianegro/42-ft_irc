/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:34:21 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/13 18:29:28 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

#define MAX_EVENTS 128

void	checkArgs(int ac) {
	if (ac != 3) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid number of arguments\n";
		std::cout << "Program should be run as follows: ./ircserv <port> <password>\n";
		exit(EXIT_FAILURE);
	}
}

void	checkPort(int port) {
	if (port < 0 || port > 65535) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid port\n";
		std::cout << "Port should be between valid range of 0 to 65535\n";
		exit(EXIT_FAILURE);
	}
	if (port < 1024) {
		std::cout << "\033[1;33mWARNING\033[0m Port " << port << " is ""well-known"" and requires special permissions to bind to\n";
	}
}

int	main(int ac, char **av) {
	checkArgs(ac);

	Server	server;

	checkPort(atoi(av[1]));
	server.setPort(atoi(av[1]));
	server.setPw(av[2]);

	server.startServer();
	server.startEpoll();
	while (true) {
		server.runEpoll();
	}
	return (0);
}
