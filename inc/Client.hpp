/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:58:07 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/07 11:58:10 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

// A client is anything connecting to a server that is not another server.

class	Client {

public:
	Client(); // ocf default constructor
	Client(const Client &src); // ocf copy constructor
	~Client(); // ocf destructor

	Client&	operator=(const Client &src); // ocf copy assignment operator

private:
	std::string	_nickname; 	// Each client is distinguished from other clients by a unique nickname.
	// in addition to the nickname, all servers must have the following information about all clients:
	int			_hostAddress; // the real name/address of the host that the client is connecting from,
	std::string	_username; // the username of the client on that host,
	int			_server; // and the server to which the client is connected.

	int							_socket; // client's socket file descriptor so we can read from and write to it
	bool						_isAuthorized; // whether the pw was correct or not
	std::vector<std::string>	_channels; // list of channels the client is currently in
	bool						_isOperator; // if the client is an operator instead of a regular user
};
