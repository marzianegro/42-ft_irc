/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:00:00 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/17 10:39:28 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Client.hpp"
#include "../incs/numReplies.hpp"

Channel::Channel() {
}

Channel::Channel(Client *creator, const std::string &name) {
	this->_opUsers.push_back(creator);
	this->_name = name;
	this->_userLimit = 2142;
	this->_userCount = 1;
	this->_iModeOn = false;
	this->_tModeOn = false;
	this->_kModeOn = false;
	this->_oModeOn = false;
	this->_lModeOn = false;
}

Channel::Channel(Client *creator, const std::string &name, const std::string &key) {
	this->_opUsers.push_back(creator);
	this->_name = name;
	this->_key = key;
	this->_userLimit = 2142;
	this->_userCount = 1;
	this->_iModeOn = false;
	this->_tModeOn = false;
	this->_kModeOn = true;
	this->_oModeOn = false;
	this->_lModeOn = false;
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

std::string	Channel::getTopic(Client *user) const {
	if (this->_topic.empty()) {
		return (rplNoTopic(this->_name, user->getNickname()));
	} else {
		return (rplTopic(this->_name, user->getNickname(), this->_topic));
	}
}

std::string	Channel::getKey() const {
	return (this->_key);
}

unsigned int	Channel::getLimit() const {
	return (this->_userLimit);
}

unsigned int	Channel::getCount() const {
	return (this->_userCount);
}

bool	Channel::getIModeStatus() const {
	return (this->_iModeOn);
}

bool	Channel::getKModeStatus() const {
	return (this->_kModeOn);
}

bool	Channel::getTModeStatus() const {
	return (this->_tModeOn);
}

std::vector<Client*>	Channel::getOps() const {
	return (this->_opUsers);
}

std::vector<Client*>	Channel::getRegs() const {
	return (this->_regUsers);
}

std::vector<Client*>	Channel::getInvs() const {
	return (this->_invUsers);
}

void	Channel::setName(const std::string &name) {
	this->_name = name;
}
 
// FIXME: add this->_msg ????????????????????????????????
void	Channel::setTopic(const std::string &topic) {
	if (topic.length() == 1 && topic[0] == ':') {
		this->_topic = ""; // REVIEW: not sure about this // REVIEW: jk i think it's because of this SENDING: :mnegro TOPIC #test :
	} else if (topic.compare(this->_topic) != 0) {
		this->_topic = topic;
	}
}

void	Channel::upCount() {
	this->_userCount++;
}

void	Channel::downCount() {
	this->_userCount--;
}

void	Channel::addUser(Client *user) {
	this->_regUsers.push_back(user);
}

void	Channel::addInvited(Client *user) {
	this->_invUsers.push_back(user);
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
		std::cout << "User " << user->getNickname() << " found in _regUsers\n";
		return (true);
	} else if (it_op != this->_opUsers.end()) {
		std::cout << "User " << user->getNickname() << " found in _opUsers\n";
		return (true);
	}
	return (false);
}

bool	Channel::isOperator(Client *user) {
	std::vector<Client*>::iterator	it_op = std::find(this->_opUsers.begin(), this->_opUsers.end(), user);

	return (it_op != this->_opUsers.end());
}

bool	Channel::isInvited(Client *user) {
	std::vector<Client*>::iterator	it_inv = std::find(this->_invUsers.begin(), this->_invUsers.end(), user);

	return (it_inv != this->_invUsers.end());
}

void	Channel::invitedJoining(Client *user) {
	std::vector<Client*>::iterator	it_inv = std::find(this->_invUsers.begin(), this->_invUsers.end(), user);

	if (it_inv != this->_invUsers.end()) {
		this->_invUsers.erase(it_inv);
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
	std::vector<Client*>::iterator	it_reg = std::find(this->_regUsers.begin(), this->_regUsers.end(), user);
	this->_regUsers.erase(it_reg);
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
	std::vector<Client*>::iterator	it_op = std::find(this->_opUsers.begin(), this->_opUsers.end(), user);
	this->_opUsers.erase(it_op);
	this->_regUsers.push_back(user);
}

void	Channel::lModeUnset() {
	this->_lModeOn = false;
	this->_userLimit = 2142;
}

std::string Channel::noModeOp() {
	std::string msg = "";
	
	if (this->_opUsers.empty() && !this->_regUsers.empty()) {
		Client *newOp = this->_regUsers.front();
		this->oModeSet(newOp);
		msg = ":gerboa MODE #" + this->getName() + " +o " + newOp->getNickname();
	}
	return (msg);
}
