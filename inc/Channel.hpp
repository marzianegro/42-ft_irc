/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:05:47 by mnegro            #+#    #+#             */
/*   Updated: 2024/03/07 11:57:54 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class	Channel {

public:
	Channel(); // ocf default constructor
	Channel(const Channel &src); // ocf copy constructor
	~Channel(); // ocf destructor

	Channel&	operator=(const Channel &src); // ocf copy assignment operator

private:
	std::string	_name;
};
