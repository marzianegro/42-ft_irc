/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/06/29 11:45:53 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iostream"
#include "unistd.h"
#include "sys/socket.h"
#include <arpa/inet.h>
#include "cstring"
#include "cstdlib"

int	main(int ac, char **av) {
	
	if (ac < 4){
		std::cout << "Usage:\n./client <IP> <PORT> \"<MESSAGE>\"" << std::endl;
		return (1);
	}

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return (1);
	}
	std::cout << "Socket created" << std::endl;
	

	in_port_t port = atoi(av[2]);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);  // Specify the server port number
	
	int pton_status = inet_pton(AF_INET, av[1], &server_addr.sin_addr);
	if ( pton_status <= 0) {
		std::cerr << "Invalid address." << std::endl;
		return (1);
	}
	std::cout << "Presentation to network successfull." << std::endl;


	int connect_status = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if ( connect_status == -1) {
		std::cerr << "Failed to connect to the server." << std::endl;
		return (1);
	}
	std::cout << "Connection to the server successfull." << std::endl;



	ssize_t bytes_sent = send(sock_fd, av[3], strlen(av[3]), 0);
	if (bytes_sent < 0)
		std::cerr << "Error while sending the message" << std::endl;
	else
		std::cerr << "Message successfully sent!" << std::endl;


	char buffer[1024];
	ssize_t bytes_read = recv(sock_fd, buffer, sizeof(buffer), 0);
	
	if (!bytes_read)
		std::cerr << "Connection closed by the server" << std::endl;
	else if (bytes_read < 0)
		std::cerr << "Error while receving the message" << std::endl;
	else {
		// // Process the received data
		buffer[bytes_read] = '\0';
		std::cout << "New message from server: " << buffer << std::endl;
	}


	close(sock_fd);
	return (0);
}
