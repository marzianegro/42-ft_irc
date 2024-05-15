/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:34:21 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/15 14:57:36 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

void	checkArgs(int ac) {
	if (ac != 3) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid number of arguments\n";
		std::cout << "Program should be run as follows: ./ircserv <port> <password>\n";
		exit(EXIT_FAILURE);
	}
}

int	checkPort(std::string portStr) {
	for (size_t i = 0; i < portStr.length(); i++) {
		if (!isdigit(portStr[i])) {
			std::cerr << "\033[1;31mERROR\033[0m Invalid port\n";
			std::cout << "Port should be a positive numeric value\n";
			exit(EXIT_FAILURE);
		}
	}
	
	int port = atoi(portStr.c_str());
	if (port < 0 || port > 65535) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid port\n";
		std::cout << "Port should be between valid range of 0 to 65535\n";
		exit(EXIT_FAILURE);
	}
	if (port < 1024) {
		std::cout << "\033[1;33mWARNING\033[0m Port " << port << " is ""well-known"" and requires special permissions to bind to\n";
	}
	return (port);
}

bool serverOn;

// NOTES: when running `nc`, use -C to automatically end with \r\n
int	main(int ac, char **av) {
	checkArgs(ac);
	signal(SIGINT, signalHandler);

	Server	server;
	bool	serverLife = true;
	
	serverStatus(&serverLife);

	server.setPort(checkPort(av[1]));
	server.setPw(av[2]);

	server.startServer();
	server.startEpoll();
	while (serverLife) {
		server.runEpoll();
	}
	return (0);
}
