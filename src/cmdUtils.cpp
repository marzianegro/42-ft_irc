/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:00:52 by mnegro            #+#    #+#             */
/*   Updated: 2024/04/19 20:20:12 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::modeSet(const std::vector<std::string> &mode, Channel *channel, Client *user) {
	std::vector<std::string>::const_iterator it_mode = mode.begin();

	if (((*it_mode)[1] == 'i')) {
		channel->iModeSet();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +i";
		this->sendToChannel(channel->getName(), NULL, false);
	} else if (((*it_mode)[1] == 't')) {
		channel->tModeSet();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +t";
		this->sendToChannel(channel->getName(), NULL, false);
	} else if (((*it_mode)[1] == 'k')) {
		++it_mode;
		if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
			channel->kModeSet(*it_mode);
			this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " +k " + *it_mode;
			this->sendToChannel(channel->getName(), NULL, false);
		} else {
			--it_mode;
		}
	} else if (((*it_mode)[1] == 'o')) {
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
	} else if (((*it_mode)[1] == 'l')) {
		++it_mode;
		if (it_mode != mode.end()) {
			std::istringstream	iss(*it_mode);
			unsigned int	tryUInt = 0;
			if (iss >> tryUInt) {
				if (tryUInt < channel->getCount() || tryUInt > 2142) {
					this->_msg = ""; // FIXME:
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
}

void Server::modeUnset(const std::vector<std::string> &mode, Channel *channel, Client *user) {
	std::vector<std::string>::const_iterator it_mode = mode.begin();

	if (((*it_mode)[1] == 'i')) {
		channel->iModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -i";
		this->sendToChannel(channel->getName(), NULL, false);
	} else if (((*it_mode)[1] == 't')) {
		channel->tModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -t";
		this->sendToChannel(channel->getName(), NULL, false);
	} else if (((*it_mode)[1] == 'k')) {
		channel->kModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -k";
		this->sendToChannel(channel->getName(), NULL, false);
	} else if (((*it_mode)[1] == 'o')) {
		++it_mode;
		if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
			std::map<int, Client*>::iterator	it_user = this->_clients.begin();

			while (it_user != this->_clients.end()) {
				if (it_user->second->getNickname().compare(*it_mode) == 0) {
					if (channel->findUser(it_user->second) && channel->isOperator(it_user->second)) {
						channel->oModeUnset(it_user->second);
						this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -o " + it_user->second->getNickname();
					} else if (channel->findUser(it_user->second) && !channel->isOperator(it_user->second)) {
						this->_msg = ":" + user->getNickname() + " NOTICE #" + channel->getName() + " :" + it_user->second->getNickname() + "is not a channel operator";
					} else if (!channel->findUser(it_user->second)) {
						this->_msg = errUserNotInChannel(channel->getName(), user->getNickname(), it_user->second->getNickname());
					}
					break;
				}
				++it_user;
			}
			if (it_user == this->_clients.end()) {
				this->_msg = errNoSuchNick(user->getNickname(), it_user->second->getNickname());
				ftSend(user->getSocket(), this->_msg);
			} if (!this->_msg.empty()) {
				this->sendToChannel(channel->getName(), NULL, false);
			}
			this->_msg = "";
		} else {
			--it_mode;
		}
	} else if (((*it_mode)[1] == 'l')) {
		channel->lModeUnset();
		this->_msg = ":" + user->getNickname() + " MODE #" + channel->getName() + " -l";
		this->sendToChannel(channel->getName(), NULL, false);
	}
}

bool checkNick(const std::string &nick) {
	for (size_t i = 0; i < nick.size(); i++) {
		if (!isalnum(nick[i]) && nick[i] != '_' && nick[i] != '-') {
			return (false);
		}
	}
	return (true);
}
