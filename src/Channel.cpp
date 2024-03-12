/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:00:00 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/12 21:37:51 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"

Channel::Channel() {
}

Channel::Channel(Client *whoOperator, const std::string &name) {
	this->_operators.push_back(whoOperator);
	whoOperator->setStatus(true); // but we probably need to specify for which channel?
	this->_name = name;
}

Channel::Channel(Client *whoOperator, const std::string &name, const std::string &key) {
	this->_operators.push_back(whoOperator);
	whoOperator->setStatus(true); // but we probably need to specify for which channel?
	this->_name = name;
	this->_key = key;
}

Channel::Channel(const Channel &src) {
	*this = src;
}

Channel::~Channel() {
}

Channel&	Channel::operator=(const Channel &src) {
	if (this != &src) {
		this->_name = src._name;
	}
	return (*this);
}

std::string	Channel::getName() const {
	return (this->_name);
}

std::string	Channel::getTopic() const {
	return (this->_topic);
}

void	Channel::setName(const std::string &name) {
	this->_name = name;
}

void	Channel::setTopic(const std::string &topic) {
	this->_topic = topic;
}

void	Channel::sendMsg(const std::string &msg) {
	ssize_t	bytesSent = send(CLIENTSOCKET, msg, msg.length(), 0); // what the fuck
	if (bytesSent == -1) {
		std::cerr << "An error occured while sending message to server\n";
		exit (EXIT_FAILURE);
	}
}

void	Channel::addClient(Client *client) {
	this->_clients.push_back(client);
}

bool	Channel::removeClient(Client *client) {
	std::vector<Client*>::iterator	it = std::find(this->_clients.begin(), this->_clients.end(), client);

	if (it != this->_clients.end()) {
		this->_clients.erase(it);
		return (true);
	} else {
		//TODO: client is not in channel so deal with it
	}
	return (false);
}

/*
	Command: KICK
  	Parameters: <channel> <user> *( "," <user> ) [<comment>]
*/
void	Channel::kick(Client *client) {
	if (removeClient(client)) {
		// success
	} else {
		// send msg + error
	}
}

/*
	Command: TOPIC
	Parameters: <channel> [<topic>]
*/
int	Channel::topic() {
	if (this->_topic.empty()) {
		sendMsg("<client> <channel> :No topic is set");
		return (331); // RPL_NOTOPIC
	} else {
		sendMsg("<client> <channel> :<topic>");
		return (332); // RPL_TOPIC
	}
}

int	Channel::topic(const std::string &topic) {
	if (topic.empty()) {
		this->_topic = ""; // why is NULL not ok?
	} else {
		this->_topic = topic;
	}
}

void	Channel::iMode() {

}

void	Channel::tMode() {

}

void	Channel::kMode() {

}

void	Channel::oMode() {

}

void	Channel::lMode() {

}
