/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:59:49 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/09 11:45:03 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Client.hpp"

Client::Client() {
}

Client::Client(const int &socket) : _socket(socket) {
	this->_isAuthorized = false;
	this->_isOperator = false;
	this->_buffer = "";
}

Client::Client(const Client &src) {
	*this = src;
}

Client::~Client() {
}

Client&	Client::operator=(const Client &src) {
	if (this != &src) {
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_realname = src._realname;
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

void	Client::setUsername(const std::string &username) {
	this->_username = username;
}

void	Client::setRealname(const std::string &realname) {
	this->_realname = realname;
}

void	Client::setAuth(const bool &auth) {
	this->_isAuthorized = auth;
}

void	Client::setStatus(const bool &status) {
	this->_isOperator = status;
}

void	Client::addChannel(const std::string &chName) {
	this->_channels.push_back(chName);
}

void	Client::removeChannel(const std::string &chName) {
	std::vector<std::string>::iterator	it_chan = std::find(this->_channels.begin(), this->_channels.end(), chName);
	
	if (it_chan != this->_channels.end()) {
		this->_channels.erase(it_chan);
	}
}

std::string Client::getNickname() const {
	return (this->_nickname);
}

std::string Client::getRealname() const {
	return (this->_realname);
}

std::string	Client::getUsername() const {
	return (this->_username);
}

int	Client::getSocket() const {
	return (this->_socket);
}

bool	Client::getAuth() const {
	return (this->_isAuthorized);
}

bool	Client::getStatus() const {
	return (this->_isOperator);
}

std::vector<std::string>	Client::getChannels() const {
	return (this->_channels);
}

void	Client::fillBuffer(const std::string &msg) {
	this->_buffer += msg;
}

std::string	Client::readBuffer() {
	std::size_t pos = this->_buffer.find("\r\n");
	
	if (pos == std::string::npos) {
		return ("");
	} else if (pos == 0) {
		this->_buffer.erase(0, 2);
		return ("");
	}

	std::string msg = this->_buffer.substr(0, pos);
	this->_buffer.erase(0, pos + 2);

	return (msg);
}
