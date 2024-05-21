/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverLife.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:28:10 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/21 12:25:04 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Client.hpp"
#include "../incs/Server.hpp"

void Server::clientDisconnect(Client *client, bool fromQuit) {	
	std::vector<std::string> channels = client->getChannels();
	std::vector<std::string>::iterator it_chan = channels.begin();
	while (it_chan != channels.end()) {
		Channel *channel = this->_channels[*it_chan];
		
		std::cout << "Disconnect: " << client->getNickname() << " is leaving " << channel->getName() << '\n';
		// REVIEW: is this part? part is already implemented though
		if (!fromQuit) {
			this->_msg = ":" + client->getNickname() + " PART #" + channel->getName() + " :Disconnected";
			sendToChannel(channel->getName(), NULL, false);
		}
		channel->downCount();
		channel->removeUser(client);
		this->checkChOperators(channel);
		client->removeChannel(channel->getName());
		++it_chan;
	}

	int clientSocket = client->getSocket();
	epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, clientSocket, &this->_current_event);
	this->_clients.erase(clientSocket);
	delete client;
	close(clientSocket);
}

void Server::newClientConnection() {
	int	clientSock = accept(this->_serverSock, NULL, NULL);
	if (clientSock == -1) {
		std::cerr << std::strerror(errno) << '\n';
		return ;
	}

	if (fcntl(clientSock, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl() failed to set client socket to non-blocking\n";
		close(clientSock);
		return ;
	}
	std::cout << "Client socket accepted\n";

	this->_event.data.fd = clientSock;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, clientSock, &this->_event) == -1) {
		std::cerr << "Failed to add file descriptor to epoll instance\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "File descriptor added to epoll instance\n";
	this->_clients[clientSock] = new Client(clientSock);
}

void Server::consoleEvent() {
	char buffer[MAX_BUFFER];
	bzero(buffer, sizeof(buffer));

	std::cin >> buffer;
	std::string console = buffer;

	std::cout << "msg from console: " + console << std::endl;
}

void Server::clientEvent(epoll_event &event) {
	char buffer[MAX_BUFFER];
	bzero(buffer, sizeof(buffer));

	ssize_t byteRecv = recv(event.data.fd, buffer, sizeof(buffer), 0);
	if (byteRecv == 0) {
		std::cout << "Connection closed by the client\n";
		std::map<int, Client*>::iterator	it = this->_clients.find(event.data.fd);
        if (it != this->_clients.end()) {
			this->clientDisconnect(it->second, false);
		}
	} else if (byteRecv == -1) {
		std::cerr << "Error while receiving the message\n";
	} else {
		std::string msg;
		std::map<int, Client*>::iterator	it = this->_clients.find(event.data.fd);
		if (it == this->_clients.end()) {
			return;
		}

		it->second->fillBuffer(buffer);
		msg = it->second->readBuffer();
		while (!msg.empty()) {
			// FIXME: problem is here!!!
			this->execCmd(msg, it->second);
			msg = it->second->readBuffer();
		}
	}
}

void	Server::runEpoll() {
	int numEvents = epoll_wait(this->_epollFd, this->_events, MAX_EVENTS, 0); // 0 for a non-blocking check
	if (numEvents == -1) {
		std::cerr << "epoll_wait() failed\n";
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < numEvents; i++) {
		this->_current_event = this->_events[i];
		std::cout << "event fd " << this->_current_event.data.fd << std::endl;
		if (this->_current_event.data.fd == this->_serverSock) { // new client connection
			this->newClientConnection();
		} else if (this->_current_event.data.fd == STDIN_FILENO) {
			this->consoleEvent(); // FIXME: ???
		} else {
			this->clientEvent(this->_events[i]);
		}
	}
}

void Server::execCmd(const std::string &msg, Client *client) {
	this->_msg = "";

	std::cout << "in execution: " << msg << std::endl;

	std::string possibleCmd[] = {"PRIVMSG", "JOIN", "INVITE", "KICK", "TOPIC", "MODE", "QUIT", "NICK", "USER", "PING", "PONG", "PART", "WHO"};
	int			lenght = sizeof(possibleCmd) / sizeof(std::string);
	std::string cmd;
	std::size_t	pos = msg.find(' ');

	cmd = msg.substr(0, pos);

	if (cmd == "CAP") {
		return ;
	} else if (!client->getAuth()) {
		if (cmd == "PASS") {
			if (this->checkPw(msg.substr(pos + 1))) {
				client->setAuth(true);
			} else {
				this->clientDisconnect(client, false);
			}
		} else {
			this->_msg = errNotRegistered();
			ftSend(client->getSocket(), this->_msg);
			this->clientDisconnect(client, false);
		}
		return ;
	}

	int cmdPos = 0;
	while (cmdPos < lenght) {
		if (cmd == possibleCmd[cmdPos]) {
			break;
		}
		cmdPos++;
	}
	
	switch (cmdPos) {
		case 0:
			this->parsePrivmsg(client, msg.substr(pos+1));
			break;
		case 1:
			this->parseJoin(client, msg.substr(pos+1));
			break;
		case 2:
			this->parseInvite(client, msg.substr(pos+1));
			break;
		case 3:
			this->parseKick(client, msg.substr(pos+1));
			break;
		case 4:
			this->parseTopic(client, msg.substr(pos+1));
			break;
		case 5:
			this->parseMode(client, msg.substr(pos+1));
			break;
		case 6:
			this->parseQuit(client, msg.substr(pos+1));
			break;
		case 7:
			this->nick(client, msg.substr(pos+1));
			break;
		case 8:
			this->parseUser(client, msg.substr(pos+1));
			break;
		case 9:
			this->ping(client, msg.substr(pos+1));
			break;
		case 10:
			this->pong(client);
			break;
		case 11:
			this->parsePart(client, msg.substr(pos+1));
			break;
		case 12:
			this->parseWho(client, msg.substr(pos+1));
			break;
		default:
			std::cout << "INTO UNKNWOWN COMMAND" << '\n';
			this->_msg = errUnknownCommand(client->getNickname(), cmd);
			ftSend(client->getSocket(), this->_msg);
			break;
	}
}

/*
**		channel.kick
**		channel.invite


	server ascolta -> riceve un messaggio -> controlla se è autenticato -> guardo se conosce il comando -> chiama il parsing
	della stringa ricevuta -> check parametri -> esegue il comando

	il comando prevede:
		- check se i parametri sono corretti e sono TUTTI (altrimenti needmodeparams)
		- check se l'utente è autorizzato a fare l'azione
		- altri check puntuali
		- esegue quel che deve fare
	
	ci sono dei comandi che agiscono sul channel, altri no:

	sul channel:
		- invite
		- kick
		- mode
		- topic
	
	altri no:
		- msg
		- join
		- quit
		- nick
		- user
		etc.
*/
