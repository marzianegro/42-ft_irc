/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:58:07 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/16 18:52:24 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
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
	void	setUsername(const std::string &username);
	void	setRealname(const std::string &realname);
	void	setAuth(const bool &auth);
	void	setStatus(const bool &status); // operator

	void	addChannel(const std::string &chName);
	void	removeChannel(const std::string &chName);

	std::string					getNickname() const;
	std::string					getUsername() const;
	std::string					getRealname() const;
	int							getSocket() const;
	bool						getAuth() const;
	bool						getStatus() const;
	std::vector<std::string>	getChannels() const;

	void		fillBuffer(const std::string &msg);
	std::string	readBuffer();

private:
	Client(); // ocf default constructor

	std::string	_nickname; 	// Each client is distinguished from other clients by a unique nickname.
	// in addition to the nickname, all servers must have the following information about all clients:
	std::string	_realname; // the real name/address of the host that the client is connecting from,
	std::string	_username; // the username of the client on that host, and the server to which the client is connected (not our case since we only have one).

	int							_socket; // client's socket file descriptor so we can read from and write to it
	bool						_isAuthorized; // whether the pw was correct or not
	std::vector<std::string>	_channels; // list of channels the client is currently in
	bool						_isOperator; // wether the client is server operator

	std::string	_buffer; // buffer for the messages
};
