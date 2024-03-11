/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:05:47 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/11 22:21:08 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

class Client;

class	Channel {

public:
	Channel(const Client &operator, const std::string &name);
	Channel(const Client &operator, const std::string &name, const std::string &key);
	Channel(const Channel &src); // ocf copy constructor
	~Channel(); // ocf destructor

	Channel&	operator=(const Channel &src); // ocf copy assignment operator

	std::string	getName() const;
	std::string	getTopic() const;

	void	setName(const std::string &name);
	void	setTopic(const std::string &topic);

	void	addClient(Client *client);
	void	removeClient(Client *client);

	// commmands
	void	kick(Client *client); // KICK
	void	invite(Client *client); // INVITE
	void	topic(std::string topic); // TOPIC
	// MODES
	void	iMode(); // -i
	void	tMode(); // -t
	void	kMode(); // -k
	void	oMode(); // -o
	void	lMode(); // -l

private:
	Channel(); // ocf default constructor

	std::string				_name;
	std::string				_topic;
	std::string				_key;
	std::vector<Client*>	_operators;
	std::vector<Client*>	_clients;
};
