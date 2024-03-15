/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:59:49 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/15 17:13:44 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() {
}

Client::Client(const int &socket) : _socket(socket) {

}

Client::Client(const Client &src) {
	*this = src;
}

Client::~Client() {
}

Client&	Client::operator=(const Client &src) {
	if (this != &src) {
		this->_nickname = src._nickname;
		this->_hostAddress = src._hostAddress;
		this->_username = src._username;
		this->_socket = src._socket;
		this->_isAuthorized = src._isAuthorized;
		this->_channels = src._channels;
		this->_isOperator = src._isOperator;
		this->_buffer = src._buffer;
	}
	return (*this);
}

void	Client::setNickname(const std::string &nickname) {
	this->_nickname = nickname;
}
void	Client::setHostAddress(const int &hostaddress) {
	this->_hostAddress = hostaddress;
}

void	Client::setUsername(const std::string &username) {
	this->_username = username;
}

void	Client::setAuth(const bool &auth) {
	this->_isAuthorized = auth;
}

void	Client::setInvitation(const bool &invite) {
	this->_isInvited = invite;
}

void	Client::setStatus(const bool &status) {
	this->_isOperator = status;
}

std::string Client::getNickname() const {
	return (this->_nickname);
}

int			Client::getHostAddress() const {
	return (this->_hostAddress);
}
std::string	Client::getUsername() const {
	return (this->_username);
}

int			Client::getSocket() const {
	return (this->_socket);
}

bool		Client::getAuth() const {
	return (this->_isAuthorized);
}

bool		Client::getInvitation() const {
	return (this->_isInvited);
}

bool		Client::getStatus() const {
	return (this->_isOperator);
}

// void		Client::fillBuffer(const std::string &msg) {

// }

// std::string	Client::readBuffer() {
	
// }
