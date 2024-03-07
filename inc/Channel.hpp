/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:05:47 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/07 12:43:10 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class Client;

class	Channel {

public:
	Channel(); // ocf default constructor
	Channel(const std::string &name);
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
	std::string				_name;
	std::string				_topic;
	Client*					_operator;
	std::vector<Client*>	_clients;
};
