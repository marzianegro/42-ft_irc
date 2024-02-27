/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:18:55 by mnegro            #+#    #+#             */
/*   Updated: 2024/02/27 19:34:55 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

// // client stages (single)
// int	main(void) {

// 	// 1. creating the client socket
// 	int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (clientSocket == -1) {
// 		std::cout << "An error occured during client socket creation\n";
// 		exit(EXIT_FAILURE);
// 	} else {
// 		std::cout << "Client socket created\n";
// 	}

// 	// 2. defining server address
// 	sockaddr_in	serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(8080);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	// 3. connecting to the server
// 	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
// 		std::cout << std::strerror(errno) << '\n';
// 		exit(EXIT_FAILURE);
// 	} else {
// 		std::cout << "Client socket connected to server\n";
// 	}

// 	// 4. sending data to server
// 	const char	*message = "Hello, server!\n";
// 	ssize_t	bytesSent = send(clientSocket, message, strlen(message), 0);
// 	if (bytesSent == -1) {
// 		std::cout << "An error occured while sending message to server\n";
// 		exit (EXIT_FAILURE);
// 	} else {
// 		std::cout << "Message sent to server: " << message;
// 	}

// 	// 5. closing the client socket
// 	close(clientSocket);

// 	return (0);
// }

// // client stages (loop)
int	main(int ac, char **av) {
	if (ac != 2) {
		std::cout << "Missing message for server\n";
		exit(EXIT_FAILURE);
	}
	// 1. creating the client socket
	int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		std::cout << "An error occured during client socket creation\n";
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Client socket created\n";
	}

	// 2. defining server address
	sockaddr_in	serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// 3. connecting to the server
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		std::cout << std::strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Client socket connected to server\n";
	}

	// 4. sending data to server
	ssize_t	bytesSent = send(clientSocket, av[1], strlen(av[1]), 0);
	if (bytesSent == -1) {
		std::cout << "An error occured while sending message to server\n";
		exit (EXIT_FAILURE);
	} else {
		std::cout << "Message sent to server: " << av[1] << '\n';
	}

	// 5. closing the client socket
	close(clientSocket);

	return (0);
}
