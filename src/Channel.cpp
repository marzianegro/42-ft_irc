/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:00:00 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/16 18:46:03 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/numReplies.hpp"

Channel::Channel() {
}

Channel::Channel(Client *creator, const std::string &name) {
	this->_opUsers.push_back(creator);
	this->_name = name;
}

Channel::Channel(Client *creator, const std::string &name, const std::string &key) {
	this->_opUsers.push_back(creator);
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

std::string	Channel::getKey() const {
	return (this->_key);
}

int	Channel::getLimit() const {
	return (this->_userLimit);
}

int	Channel::getCount() const {
	return (this->_userCount);
}

bool	Channel::getIModeStatus() const {
	return (this->_iModeOn);
}

bool	Channel::getKModeStatus() const {
	return (this->_kModeOn);
}

std::vector<Client*>	Channel::getOps() const {
	return (this->_opUsers);
}

std::vector<Client*>	Channel::getRegs() const {
	return (this->_regUsers);
}

void	Channel::setName(const std::string &name) {
	this->_name = name;
}

void	Channel::setTopic(const std::string &topic) {
	this->_topic = topic;
}

void	Channel::setCount() {
	this->_userCount++;
}

void	Channel::addUser(Client *user) {
	this->_regUsers.push_back(user);
}

bool	Channel::removeUser(Client *user) {
	std::vector<Client*>::iterator	it_reg = std::find(this->_regUsers.begin(), this->_regUsers.end(), user);
	std::vector<Client*>::iterator	it_op = std::find(this->_opUsers.begin(), this->_opUsers.end(), user);

	if (it_reg != this->_regUsers.end()) {
		this->_regUsers.erase(it_reg);
		return (true);
	} else if (it_op != this->_opUsers.end()) {
		this->_opUsers.erase(it_op);
		return (true);
	}
	return (false);
}

bool	Channel::findUser(Client *user) {
	std::vector<Client*>::iterator	it_reg = std::find(this->_regUsers.begin(), this->_regUsers.end(), user);
	std::vector<Client*>::iterator	it_op = std::find(this->_opUsers.begin(), this->_opUsers.end(), user);

	if (it_reg != this->_regUsers.end()) {
		return (true);
	} else if (it_op != this->_opUsers.end()) {
		return (true);
	}
	return (false);
}

std::string	Channel::kick(Client *kicker, Client *kicked, const std::string &reason) {
	// TODO: check if client is NULL
	if (!removeUser(kicked)) {
		return (errNotOnChannel(this->_name, kicked->getNickname()));
	}
	return (NULL);
}

std::string	Channel::topic(Client *user) {
	if (this->_topic.empty()) {
		return (rplNoTopic(this->_name, user->getNickname()));
	} else {
		return (rplTopic(this->_name, user->getNickname(), this->_topic));
	}
}

void	Channel::topic(Client *user, const std::string &topic) {
	if (this->_tModeOn && !user->getStatus()) {
		// TODO: user must have channel privilege operator status in order to change the topic
		return; // error?
	}
	if (topic.empty()) {
		this->_topic = "";
	} else if (!topic.compare(this->_topic)) {
		this->_topic = topic;
	}
}

void	Channel::iModeSet() {
	this->_iModeOn = true;
}

void	Channel::tModeSet() {
	this->_tModeOn = true;
}

void	Channel::kModeSet(const std::string &key) {
	this->_kModeOn = true;
	this->_key = key;
}

void	Channel::oModeSet(Client *user) {
	this->_oModeOn = true;
	std::vector<Client*>::iterator	regIT = std::find(this->_regUsers.begin(), this->_regUsers.end(), user);
	this->_regUsers.erase(regIT);
	this->_opUsers.push_back(user);
}

void	Channel::lModeSet(const int &limit) {
	this->_lModeOn = true;
	this->_userLimit = limit;
}

void	Channel::iModeUnset() {
	this->_iModeOn = false;
}

void	Channel::tModeUnset() {
	this->_tModeOn = false;
}

void	Channel::kModeUnset() {
	this->_kModeOn = false;
	this->_key = "";
}

void	Channel::oModeUnset(Client *user) {
	this->_oModeOn = false;
	std::vector<Client*>::iterator	opIT = std::find(this->_opUsers.begin(), this->_opUsers.end(), user);
	this->_opUsers.erase(opIT);
	this->_regUsers.push_back(user);
}

void	Channel::lModeUnset() {
	this->_lModeOn = false;
	this->_userLimit = 42; // TODO: not sure
}
