/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:00:52 by mnegro            #+#    #+#             */
/*   Updated: 2024/04/19 10:30:10 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

void Server::modeSet(const std::vector<std::string> &mode, Channel *channel, Client *user) {
	std::vector<std::string>::const_iterator it_mode = mode.begin();

	if (((*it_mode)[1] == 'i')) {
		channel->iModeSet();
	} else if (((*it_mode)[1] == 't')) {
		channel->tModeSet();
	} else if (((*it_mode)[1] == 'k')) {
		++it_mode;
		if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
			channel->kModeSet(*it_mode);
		} else {
			--it_mode;
		}
	} else if (((*it_mode)[1] == 'o')) {
		++it_mode;
		std::cout << "1. The user we're trying to make operator is " << *(it_mode) << '\n';
		if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
			std::map<int, Client*>::iterator	it_user = this->_clients.begin();

			while (it_user != this->_clients.end()) {
				std::cout << "We're looking in this->_clients to find " << *(it_mode) << '\n';
				std::cout << "Right now we're at user " << it_user->second->getNickname() << '\n';
				if (it_user->second->getNickname().compare(*it_mode) == 0) {
					if (channel->findUser(it_user->second) && !channel->isOperator(it_user->second)) { // REVIEW: no error msg for already being operator
						channel->oModeSet(it_user->second);
						std::cout << it_user->second->getNickname() << " is now channel operator\n"; // FIXME: change to this->_msg
					} else if (channel->findUser(it_user->second) && channel->isOperator(it_user->second)) {
						std::cout << it_user->second->getNickname() << " is already channel operator\n"; // FIXME: change to this->_msg
					} else if (!channel->findUser(it_user->second)) {
						this->_msg = errUserNotInChannel(channel->getName(), user->getNickname(), it_user->second->getNickname());
					}
					break;
				}
				++it_user;
			}
			if (it_user == this->_clients.end()) {
				this->_msg = errNoSuchNick(user->getNickname(), it_user->second->getNickname());
			}
			if (!this->_msg.empty()) {
				ftSend(user->getSocket(), this->_msg);
				this->_msg = "";
			}
		} else {
			--it_mode;
		}
	} else if (((*it_mode)[1] == 'l')) {
		++it_mode;
		if (it_mode != mode.end()) {
			std::istringstream	iss(*it_mode);
			unsigned int	tryUInt = 0;
			if (iss >> tryUInt) {
				channel->lModeSet(tryUInt);
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
	} else if (((*it_mode)[1] == 't')) {
		channel->tModeUnset();
	} else if (((*it_mode)[1] == 'k')) {
		channel->kModeUnset();
	} else if (((*it_mode)[1] == 'o')) {
		++it_mode;
		std::cout << "++it_mode results in " << (*it_mode) << '\n';
		if (it_mode != mode.end() && it_mode->length() > 0 && ((*it_mode)[0] != '+' && (*it_mode)[0] != '-')) {
			std::map<int, Client*>::iterator	it_user = this->_clients.begin();

			while (it_user++ != this->_clients.end()) {
				if (it_user->second->getNickname().compare(*it_mode) == 0) {
					if (channel->findUser(it_user->second) && channel->isOperator(it_user->second)) {
						channel->oModeUnset(it_user->second);
						std::cout << it_user->second->getNickname() << " is no longer channel operator\n"; // FIXME: change to this->_msg
					} else if (channel->findUser(it_user->second) && !channel->isOperator(it_user->second)) {
						std::cout << it_user->second->getNickname() << " is not channel operator to begin with\n"; // FIXME: change to this->_msg
					} else if (!channel->findUser(it_user->second)) {
						this->_msg = errUserNotInChannel(channel->getName(), user->getNickname(), it_user->second->getNickname());
					}
					break;
				}
			}
			if (it_user == this->_clients.end()) {
				this->_msg = errNoSuchNick(user->getNickname(), it_user->second->getNickname());
			}
			if (!this->_msg.empty()) {
				ftSend(user->getSocket(), this->_msg);
				this->_msg = "";
			}
		} else {
			--it_mode;
		}
	} else if (((*it_mode)[1] == 'l')) {
		channel->lModeUnset();
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
