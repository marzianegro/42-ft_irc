/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:18:55 by mnegro            #+#    #+#             */
/*   Updated: 2024/02/27 18:02:03 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

// client stages
int	main(void) {

	// 1. creating the client socket
	int	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		std::cout << "An error occured during client socket creation\n";
		exit(EXIT_FAILURE);
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
	}

	// 4. sending data to server
	const char	*message = "Hello, server!\n";
	ssize_t	bytesSent = send(clientSocket, message, strlen(message), 0);
	if (bytesSent == -1) {
		std::cout << "An error occured while sending message to server\n";
		exit (EXIT_FAILURE);
	} else {
		std::cout << "Message was successfully sent to server\n";
	}

	// 5. closing the client socket
	close(clientSocket);

	return (0);
}
