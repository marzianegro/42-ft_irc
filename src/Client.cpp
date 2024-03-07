/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:59:49 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/07 12:23:50 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() {
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
		this->_server = src._server;
		this->_socket = src._socket;
		this->_isAuthorized = src._isAuthorized;
		this->_channels = src._channels;
		this->_isOperator = src._isOperator;
	}
	return (*this);
}
