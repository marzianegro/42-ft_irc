/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:21:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/16 00:49:21 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include "numReplies.hpp"
#include <signal.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>

#define MAX_BUFFER	512
#define MAX_EVENTS	128

class Channel;
class Client;

class	Server {

public:
	Server(); // ocf default constructor
	Server(const Server &src); // ocf copy constructor
	~Server(); // ocf destructor

	Server&	operator=(const Server &src); // ocf copy assignment operator

	void	setPort(const in_port_t &port);
	void	setPw(const std::string &pw);

	in_port_t						getPort() const;
	std::string						getPw() const;
	int								getSocket() const;
	int								getEpollFd() const;
	std::map<int, Client*>			getClients() const;
	std::map<std::string, Channel*>	getChannels() const;

	void	startServer();
	void	startEpoll();
	void	runEpoll();

	void	invite(Client *inviter, Client *invited, const std::string &channel);
	void	quit(Client *client, std::string reason);
	
private:  
	void	newClientConnection();
	void	clientEvent(epoll_event &event);
	void	consoleEvent();
	void	execCmd(const std::string &msg, Client *client);
	bool	checkPw(const std::string &pw);
	bool	checkNicknames(const std::string &nickname);
	bool	checkUsernames(const std::string &username);
	void	clientDisconnect(Client *client, bool fromQuit);
	void	sendToChannel(const std::string &chName, Client *exclude, bool onlyOps);
	void	sendToEveryone(Client *exclude);
	void	checkChOperators(Channel *channel);

	void	sendMsgToClient(Client *client, const std::string &target, std::string &msg);
	void	sendMsgToChannel(Client *client, std::string channel, std::string &msg, bool onlyOps);
	void	join(Client *user, std::string &chName, const std::string &key);
	void	part(Client *user, std::string &chName, std::string reason);
	void	kick(Client *kicker, Client *kicked, std::string &chName, const std::string &reason);
	void	topic(Client *user, const std::string &channel, const std::string &topic);
	void	mode(Client *user, const std::string &channel, const std::vector<std::string> &mode);

	void 	modeSet(const std::vector<std::string> &mode, Channel *channel, Client *user);
	void 	iModeSet(Channel *channel, Client *user);
	void 	tModeSet(Channel *channel, Client *user);
	void 	kModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode);
	void 	oModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode);
	void 	lModeSet(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode);

	void 	modeUnset(const std::vector<std::string> &mode, Channel *channel, Client *user);
	void 	iModeUnset(Channel *channel, Client *user);
	void 	tModeUnset(Channel *channel, Client *user);
	void 	kModeUnset(Channel *channel, Client *user);
	void 	oModeUnset(const std::vector<std::string> &mode, Channel *channel, Client *user, std::vector<std::string>::const_iterator it_mode);
	void 	lModeUnset(Channel *channel, Client *user);

	void	nick(Client *client, const std::string &newNick);
	void	user(Client *client, const std::string &username, const std::string &realname);
	void	ping(Client *client, const std::string &token);
	void	pong(Client *client);
	void	who(Client *client, const std::string &chName);

	void	parsePrivmsg(Client *client, std::string msg);
	void	parseJoin(Client *client, std::string msg);
	void	parseInvite(Client *client, std::string msg);
	void	parseKick(Client *client, std::string msg);
	void	parseTopic(Client *client, std::string msg);
	void	parseMode(Client *client, std::string msg);
	void	parseQuit(Client *client, std::string msg);
	void	parseUser(Client *client, std::string msg);
	void	parseOper(Client *client, std::string msg);
	void	parsePart(Client *client, std::string msg);
	void	parseWho(Client *client, std::string mask);

	Client*	findClientByNick(const std::string &nick);

	std::string			_msg;

	in_port_t			_port;
	std::string			_pw;

	int					_serverSock;
	sockaddr_in			_serverAddr;

	int					_epollFd;
	struct epoll_event	_event; // specifying the type of events to monitor
	struct epoll_event	_events[MAX_EVENTS]; // array to hold events returned by epoll_wait()
	struct epoll_event	_current_event;

	std::map<int, Client*>			_clients; // client's fd and object
	std::map<std::string, Channel*>	_channels; // channel's name and object
};

// Utils
bool		checkNick(const std::string &nick);
std::string fixChannelName(const std::string &channelName);
void		ftSend(int fd, std::string &msg);
void		serverStatus(bool *serverSwitch);
bool 		isChannelValid(const std::string &channel);
bool 		isNicknameValid(const std::string &nickname);
void		signalHandler(int signal);
std::string	trimChannelName(const std::string &channel);
std::string toString(int n);
std::string toString(unsigned int n);
