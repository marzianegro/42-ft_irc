/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:05:47 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/12 21:37:51 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

class Client;

class	Channel {

public:
	// for these constructors whoOperator cannot be const, as we need to be
	// able to modify its data in Channel
	Channel(Client *whoOperator, const std::string &name);
	Channel(Client *whoOperator, const std::string &name, const std::string &key);
	Channel(const Channel &src); // ocf copy constructor
	~Channel(); // ocf destructor

	Channel&	operator=(const Channel &src); // ocf copy assignment operator

	std::string				getName() const;
	std::string				getTopic() const;
	std::vector<Client*>	getOps() const;
	std::vector<Client*>	getRegs() const;

	void	setName(const std::string &name);
	void	setTopic(const std::string &topic);

	// new entry
	void	sendMsg(const std::string &msg);

	void	addClient(Client *client);
	bool	removeClient(Client *client);

	// commmands
	int	kick(Client *client); // KICK
	int	topic(); // TOPIC without <topic>
	int	topic(const std::string &topic); // TOPIC with <topic>
	// MODES
	int	iMode(); // -i
	int	tMode(); // -t
	int	kMode(); // -k
	int	oMode(); // -o
	int	lMode(); // -l

private:
	Channel(); // ocf default constructor

	std::string				_name;
	std::string				_topic;
	std::string				_key;
	std::vector<Client*>	_opUsers;
	std::vector<Client*>	_regUsers;
};
