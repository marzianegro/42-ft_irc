/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:34:21 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/06 21:55:09 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX_EVENTS 128

void	checkArgs(int ac) {
	if (ac != 3) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid number of arguments\n";
		std::cout << "Program should be run as follows: ./ircserv <port> <password>\n";
		exit(EXIT_FAILURE);
	}
}

void	checkPort(in_port_t port) {
	if (port < 0 || port > 65535) {
		std::cerr << "\033[1;31mERROR\033[0m Invalid port\n";
		std::cout << "Port should be between valid range of 0 to 65535\n";
		exit(EXIT_FAILURE);
	}
	if (port < 1024) {
		std::cout << "\033[1;33mWARNING\033[0m Port " << port << " is ""well-known"" and requires special permissions to bind to\n";
	}
}

bool	checkClient(int client) {
	
}

int	main(int ac, char **av) {
	checkArgs(ac);

	in_port_t	port = atoi(av[1]);
	checkPort(port);
	std::string	pw = av[2];

	int	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "An error occured during server socket creation\n";
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Server socket created\n";
	}

	sockaddr_in	servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(port);
	servAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr*)&servAddress, sizeof(servAddress)) == -1) {
		std::cerr << std::strerror(errno) << '\n';
		close(serverSocket);
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Server socket bound\n";
	}

	if (listen(serverSocket, 128) == -1) {
		std::cerr << std::strerror(errno) << '\n';
		close(serverSocket);
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Server socket now listening\n";
	}

	int	fd_epoll = epoll_create1(0);
	if (fd_epoll == -1) {
		std::cerr << "An error occured during epoll instance creation\n";
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Epoll instance created\n";
	}

	struct epoll_event event; // structure to specify the type of events i'm interested in
	event.events = EPOLLIN;
	event.data.fd = serverSocket;

	if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
		std::cerr << "Failed to add file descriptor to epoll instance\n";
		exit(EXIT_FAILURE);
	} else {
		std::cout << "File descriptor added to epoll instance\n";
	}

	struct epoll_event events[MAX_EVENTS]; // array to hold the events that epoll_wait returns
	while (true) {
		int	numEvents = epoll_wait(fd_epoll, event, MAX_EVENTS, 0); // 0 for a non-blocking check
		if (numEvents == -1) {
			std::cerr << "epoll_wait() failed\n";
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < numEvents; i++) {
			if (events[i].data.fd == serverSocket) {
				int	clientSocket = accept(serverSocket, NULL, NULL);
				if (clientSocket == -1) {
					std::cerr << std::strerror(errno) << '\n';
					continue ;
				}
				if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1) {
					std::cerr << "fcntl() failed to set client socket to non-blocking\n";
					close(clientSocket);
					continue ;
				}
				std::cout << "Client socket accepted\n";
				event.data.fd = clientSocket;
				if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
					std::cerr << "Failed to add file descriptor to epoll instance\n";
					exit(EXIT_FAILURE);
				}
				std::cout << "File descriptor added to epoll instance\n";
			} else {
				int	clientSocket = events[i].data.fd;
				// handle client data
			}
		}
	}
	return (0);
}

if (checkClient(clientSocket)) {
	// this is where i check for the password
} else {
	close(clientSocket);
	continue ;
}
