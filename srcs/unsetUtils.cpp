/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:58:43 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/25 00:26:47 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Client.hpp"
#include "../incs/Server.hpp"

void Server::modeUnset(const std::vector<std::string> &mode, Channel *channel, Client *user) {
	std::vector<std::string>::const_iterator it_mode = mode.begin();

	if (((*it_mode)[1] == 'i')) {
		this->iModeUnset(channel, user);
	} else if (((*it_mode)[1] == 't')) {
		this->tModeUnset(channel, user);
	} else if (((*it_mode)[1] == 'k')) {
		this->kModeUnset(channel, user);
	} else if (((*it_mode)[1] == 'o')) {
		this->oModeUnset(mode, channel, user, it_mode);
	} else if (((*it_mode)[1] == 'l')) {
		this->lModeUnset(channel, user);
	}
	this->_msg = "";
}


void 	Server::iModeUnset(Channel *channel, Client *user) {
	if (!channel->getIModeStatus()) {
		this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " : Invite-Only (+i) mode is not enabled";
	} else {
		channel->iModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -i : | Invite-Only disabled";
	}
	this->sendToChannel(channel->getName(), NULL, false);
}

void 	Server::tModeUnset(Channel *channel, Client *user) {
	if (!channel->getTModeStatus()) {
		this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " : Protected Topic (+t) mode is not enabled";
	} else {
		channel->tModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -t : | Protected Topic disabled";
	}
	this->sendToChannel(channel->getName(), NULL, false);
}

void 	Server::kModeUnset(Channel *channel, Client *user) {
	if (!channel->getKModeStatus()) {
		this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " : Channel Key (+k) mode is not enabled";
	} else {
		channel->kModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -k";
	}
	this->sendToChannel(channel->getName(), NULL, false);
}

void 	Server::oModeUnset(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode) {
	++it_mode;
	if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
		std::map<int, Client*>::iterator	it_user = this->_clients.begin();

		while (it_user != this->_clients.end()) {
			if (it_user->second->getNickname().compare(*it_mode) == 0) {
				if (channel->getOps().size() == 1) {
					this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " :You're the last operator left in channel, can't DeOp yourself";
				} else if (channel->findUser(it_user->second) && channel->isOperator(it_user->second)) {
					channel->oModeUnset(it_user->second);
					this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -o " + it_user->second->getNickname();
				} else if (channel->findUser(it_user->second) && !channel->isOperator(it_user->second)) {
					this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " :" + it_user->second->getNickname() + " is not a channel operator";
				} else if (!channel->findUser(it_user->second)) {
					this->_msg = errUserNotInChannel(channel->getName(), user->getNickname(), it_user->second->getNickname());
				}
				break;
			}
			++it_user;
		}
		if (it_user == this->_clients.end()) {
			this->_msg = errNoSuchNick(user->getNickname(), *it_mode);
			ftSend(user->getSocket(), this->_msg);
		} if (!this->_msg.empty()) {
			this->sendToChannel(channel->getName(), NULL, false);
		}
	} else {
		--it_mode;
	}
}

void 	Server::lModeUnset(Channel *channel, Client *user) {
	if (!channel->getLModeStatus()) {
		this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " : User Limit (+l) mode is not enabled";
	} else {
		channel->lModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -l";
	}
	this->sendToChannel(channel->getName(), NULL, false);
}
