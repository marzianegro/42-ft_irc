/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:19:01 by mnegro            #+#    #+#             */
/*   Updated: 2024/02/27 18:04:32 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

// server stages
int	main(void) {

	// 1. creating the server socket
	int	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // AF_INET
	if (serverSocket == -1) {
		std::cout << "An error occured during server socket creation\n";
		exit(EXIT_FAILURE);
	}

	// 2. defining server address
	sockaddr_in	serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	// option 1 is to bind the socket to a particular IP
	// const char	*ipAddress = "192.168.0.1";
	// if (inet_pton(AF_INET, ipAddress, &(serverAddress.sin_addr) <= 0) {
	// 	std::cout << "Invalid IP address format OR Conversion error\n";
	// 	exit (EXIT_FAILURE);
	// }
	// option 2 is to make the socket bind and listen to all the available IPs
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// 3. binding the server socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		std::cout << std::strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	}

	// 4. listening for connections
	if (listen(serverSocket, 5) == -1) {
		std::cout << std::strerror(errno) << '\n';
		exit(EXIT_FAILURE);
	}

	// 5. accepting a client connection
	int	clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == -1) {
		std::cout << "An error occured during client socket creation\n";
		exit(EXIT_FAILURE);
	}

	// 6. receiving data from the client
	char	buffer[1024] = {0};
	ssize_t	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead == -1) {
		std::cout << "An error occuring while receiving data from client\n";
	} else if (bytesRead == 0) {
		std::cout << "Connection closed by remote side\n";
	} else {
		std::cout << "Message from client: " << buffer << '\n';
	}

	// 7. closing the server socket
	close(serverSocket);

	return (0);
}
