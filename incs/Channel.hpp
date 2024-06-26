/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:05:47 by mnegro            #+#    #+#             */
/*   Updated: 2024/05/24 21:13:42 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

class Client;

class	Channel {

public:
	Channel(Client *creator, const std::string &name);
	Channel(Client *creator, const std::string &name, const std::string &key);
	Channel(const Channel &src); // ocf copy constructor
	~Channel(); // ocf destructor

	Channel&	operator=(const Channel &src); // ocf copy assignment operator

	std::string				getName() const;
	std::string				getTopic(Client *user) const;
	std::string				getKey() const;
	unsigned int			getLimit() const;
	unsigned int			getCount() const;
	bool					getIModeStatus() const;
	bool					getKModeStatus() const;
	bool					getTModeStatus() const;
	bool					getLModeStatus() const;
	std::vector<Client*>	getOps() const;
	std::vector<Client*>	getRegs() const;
	std::vector<Client*>	getInvs() const;

	void	setName(const std::string &name);
	void	setTopic(const std::string &topic);
	
	void	upCount();
	void	downCount();

	void		addUser(Client *user);
	void		addInvited(Client *user);
	bool		removeUser(Client *user);
	bool		findUser(Client *user);
	bool		isOperator(Client *user);
	bool		isInvited(Client *user);
	void		invitedJoining(Client *user);
	std::string	noModeOp();

	void		iModeSet();
	void		tModeSet();
	void		kModeSet(const std::string &key);
	void		oModeSet(Client *user);
	void		lModeSet(const int &limit);
	void		iModeUnset();
	void		tModeUnset();
	void		kModeUnset();
	void		oModeUnset(Client *user);
	void		lModeUnset();

private:
	Channel(); // ocf default constructor

	std::string		_name;
	std::string		_topic;
	std::string		_key;
	unsigned int	_userLimit;
	unsigned int	_userCount;

	bool	_iModeOn; // invite-only channel mode
	bool	_tModeOn; // protected topic mode
	bool	_kModeOn; // key channel mode
	bool	_oModeOn; // oper user mode
	bool	_lModeOn; // client limit channel mode

	std::vector<Client*>	_opUsers; // operator (not regular)
	std::vector<Client*>	_regUsers; // regular (not operator)
	std::vector<Client*>	_invUsers; // invited
};
