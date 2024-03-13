/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 21:34:28 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/13 18:57:14 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"

// TODO:
// CAP message
// AUTHENTICATE message
// PASS message
// NICK message
// USER message
// PING message
// PONG message
// OPER message
// QUIT message
// ERROR message

// DONE
// JOIN message
// PART message
// TOPIC message
// NAMES message
// LIST message
// INVITE message
// Invite list
// KICK message

// ERR_NOSUCHCHANNEL (403)
// ERR_USERNOTINCHANNEL (441)
// ERR_NOTONCHANNEL (442)
// ERR_USERONCHANNEL (443)
// ERR_NEEDMOREPARAMS (461)
// ERR_CHANNELISFULL (471)
// ERR_INVITEONLYCHAN (473)
// ERR_BADCHANNELKEY (475)
// ERR_CHANOPRIVSNEEDED (482)

// RPL_NOTOPIC (331)
// RPL_TOPIC (332)
// RPL_INVITING (341)
// RPL_NAMREPLY (353)
// RPL_ENDOFNAMES (366)

// ERR

std::string errNoSuchChannel(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 403 " + clientName + " " + channelName + " :No such channel");
}

std::string errUserNotInChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName) {
	return (":gerboa 441 " + clientName + " " + invitedClientName + " " + channelName + " :They aren't on that channel");
}

std::string errNotOnChannel(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 442 " + clientName + " " + channelName + " :You're not on that channel");
}

std::string errUserOnChannel(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName) {
	return (":gerboa 443 " + clientName + " " + invitedClientName + " " + channelName + " :is already on channel");
}

std::string errNeedMoreParams(const std::string &clientName, const std::string &command) {
	return (":gerboa 461 " + clientName + " " + command + " :Not enough parameters");
}

std::string errChannelIsFull(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 471 " + clientName + " " + channelName + " :Cannot join channel (+l)");
}

std::string errInviteOnlyChan(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 473 " + clientName + " " + channelName + " :Cannot join channel (+i)");
}

std::string errBadChannelKey(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 475 " + clientName + " " + channelName + " :Cannot join channel (+k)");
}

std::string errChanOPrivsNeeded(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 482 " + clientName + " " + channelName + " :You're not channel operator");
}


// RPL

std::string rplNoTopic(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 331 " + clientName + " " + channelName + " :No topic is set");
}

std::string rplTopic(const std::string &channelName, const std::string &clientName, const std::string &topic) {
	return (":gerboa 332 " + clientName + " " + channelName + " :" + topic);
}

std::string rplInviting(const std::string &channelName, const std::string &clientName, const std::string &invitedClientName) {
	return (":gerboa 341 " + clientName + " " + invitedClientName + " " + channelName);
}

std::string rplNamReply(const Channel &channel, const std::string &clientName) {
	std::string names;
	std::vector<Client*> clients = channel.getOps();

	std::vector<Client*>::const_iterator it = clients.begin();
	while (it != clients.end()) {
		names += "@" + (*it)->getNickname() + " ";
		++it;
	}

	names += " ";

	clients = channel.getRegs();
	it = clients.begin();
	while (it != clients.end()) {
		names += (*it)->getNickname() + " ";
		++it;
	}

	return (":gerboa 353 " + clientName + " = " + channel.getName() + " :" + names);
}

std::string rplEndOfNames(const std::string &channelName, const std::string &clientName) {
	return (":gerboa 366 " + clientName + " " + channelName + " :End of /NAMES list");
}
