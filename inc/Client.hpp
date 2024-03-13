/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:58:07 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/13 21:00:08 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

class Channel;
// A client is anything connecting to a server that is not another server.

class	Client {

public:
	Client(const int &socket);
	Client(const Client &src); // ocf copy constructor
	~Client(); // ocf destructor

	Client&	operator=(const Client &src); // ocf copy assignment operator

	void	setNickname(const std::string &nickname);
	void	setHostAddress(const int &hostaddress);
	void	setUsername(const std::string &username);
	void	setAuth(const bool &auth);
	void	setInvitation(const bool &invite);
	void	setStatus(const bool &status); // operator

	std::string getNickname() const;
	int			getHostAddress() const;
	std::string	getUsername() const;
	int			getSocket() const;
	bool		getAuth() const;
	bool		getInvitation() const;
	bool		getStatus() const;

	// void		fillBuffer(const std::string &msg);
	// std::string	readBuffer();

private:
	Client(); // ocf default constructor

	std::string	_nickname; 	// Each client is distinguished from other clients by a unique nickname.
	// in addition to the nickname, all servers must have the following information about all clients:
	int			_hostAddress; // the real name/address of the host that the client is connecting from,
	std::string	_username; // the username of the client on that host, and the server to which the client is connected (not our case since we only have one).

	int							_socket; // client's socket file descriptor so we can read from and write to it
	bool						_isAuthorized; // whether the pw was correct or not
	bool						_isInvited;
	std::vector<std::string>	_channels; // list of channels the client is currently in
	bool						_isOperator; // if the client is the server operator

	std::string	_buffer; // buffer for the messages
};
