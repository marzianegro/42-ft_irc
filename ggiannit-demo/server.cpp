/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/06/29 16:19:48 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iostream"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <cstring>
#include <csignal>


int sock_fd = 0;
int	server_life = 1;

void	signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	server_life = 0;
	close(sock_fd);
}

int	main() {
	in_port_t port = 8082;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
	std::cout << "Socket created" << std::endl;
	
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY; // Any IP address
	server_addr.sin_port = htons(port); // Port

	int bind_status = bind(sock_fd, (sockaddr *)&server_addr, sizeof(server_addr));
	if (bind_status) {
		std::cerr << "Failed to bind socket to address" << std::endl;
		close(sock_fd);
		return 1;
	}
	std::cout << "Socket bound to address" << std::endl;
	
	int listen_status = listen(sock_fd, 128);
	if (listen_status){
		std::cerr << "Failed to listen for connections" << std::endl;
		close(sock_fd);
		return 1;
	}
	std::cout << "Socket ready to listen for connections" << std::endl;	


	signal(SIGINT, signalHandler);

	while(server_life) {
		std::cout << std::endl;

		sockaddr_in client_addr;
		socklen_t	client_addr_len = sizeof(client_addr);
		
		int client_fd = accept(sock_fd, (sockaddr *)&client_addr, &client_addr_len);
		if (client_fd < 0) {
			std::cerr << "Failed to accept the client" << std::endl;
			close(client_fd);
			continue;
		}

		char buffer[1024];
		ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
		
		if (!bytes_read)
			std::cerr << "Connection closed by the client" << std::endl;
		else if (bytes_read < 0)
			std::cerr << "Error while receving the message" << std::endl;
		else {
			buffer[bytes_read] = '\0';
			std::cout << "New message from client: " << buffer << std::endl;
		}


		const char *message = "Hello, from server!";
		ssize_t bytes_sent = send(client_fd, message, strlen(message), 0);
		
		if (bytes_sent < 0)
			std::cerr << "Error while sending the message" << std::endl;
		else
			std::cerr << "Message successfully sent!" << std::endl;

		close(client_fd);
	}
	close(sock_fd);
	return (0);
}
