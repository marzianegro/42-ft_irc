/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:00:52 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/14 23:56:08 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Client.hpp"
#include "../incs/Server.hpp"

void Server::modeSet(const std::vector<std::string> &mode, Channel *channel, Client *user) {
	std::vector<std::string>::const_iterator it_mode = mode.begin();

	if (((*it_mode)[1] == 'i')) {
		this->iModeSet(channel, user);
	} else if (((*it_mode)[1] == 't')) {
		this->tModeSet(channel, user);
	} else if (((*it_mode)[1] == 'k')) {
		this->kModeSet(mode, channel, user, it_mode);
	} else if (((*it_mode)[1] == 'o')) {
		this->oModeSet(mode, channel, user, it_mode);
	} else if (((*it_mode)[1] == 'l')) {
		this->lModeSet(mode, channel, user, it_mode);
	}
}

void 	Server::iModeSet(Channel *channel, Client *user) {
	channel->iModeSet();
	this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +i";
	this->sendToChannel(channel->getName(), NULL, false);
}

void 	Server::tModeSet(Channel *channel, Client *user) {
	channel->tModeSet();
	this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +t";
	this->sendToChannel(channel->getName(), NULL, false);
}

void 	Server::kModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode) {
	++it_mode;
	if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
		channel->kModeSet(*it_mode);
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +k " + *it_mode;
		this->sendToChannel(channel->getName(), NULL, false);
	} else {
		--it_mode;
	}
}

void 	Server::oModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode) {
	++it_mode;
	if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
		std::map<int, Client*>::iterator	it_user = this->_clients.begin();

		while (it_user != this->_clients.end()) {
			if (it_user->second->getNickname().compare(*it_mode) == 0) {
				if (channel->findUser(it_user->second) && !channel->isOperator(it_user->second)) { // REVIEW: no error msg for already being operator
					channel->oModeSet(it_user->second);
					this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +o " + it_user->second->getNickname();
				} else if (channel->findUser(it_user->second) && channel->isOperator(it_user->second)) {
					this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " :" + it_user->second->getNickname() + "is already a channel operator";
				} else if (!channel->findUser(it_user->second)) {
					this->_msg = errUserNotInChannel(channel->getName(), user->getNickname(), it_user->second->getNickname());
					ftSend(user->getSocket(), this->_msg);
					return ;
				}
				break;
			}
			++it_user;
		}
		if (it_user == this->_clients.end()) {
			this->_msg = errNoSuchNick(user->getNickname(), it_user->second->getNickname());
			ftSend(user->getSocket(), this->_msg);
		} else if (!this->_msg.empty()) {
			this->sendToChannel(channel->getName(), NULL, false);
		}
		this->_msg = "";
	} else {
		--it_mode;
	}
}

void 	Server::lModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode) {
	++it_mode;
	if (it_mode != mode.end()) {
		std::istringstream	iss(*it_mode);
		unsigned int	tryUInt = 0;
		if (iss >> tryUInt) {
			if (tryUInt < channel->getCount() || tryUInt > 2142) {
				this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " :Te limits must be between " + toString(channel->getCount()) + " and 2142";
			} else {
				channel->lModeSet(tryUInt);
				this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +l " + *it_mode;
				this->sendToChannel(channel->getName(), NULL, false);
			}
		}
	} else {
		--it_mode;
	}
}
