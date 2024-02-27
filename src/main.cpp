/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnegro <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:34:21 by mnegro            #+#    #+#             */
/*   Updated: 2024/02/27 10:43:59 by mnegro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

void	checkArgs(int ac) {
	if (ac != 3) {
		std::cout << "\033[1;31mERROR\033[0m Invalid number of arguments\n";
		exit(1);
	}
}

int	main(int ac, char **av) {
	checkArgs(ac);
}
