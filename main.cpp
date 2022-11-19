/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 15:14:24 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/19 03:17:52 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int     main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Bad Arguments!" << std::endl;
		return(1);
	}
	Servers			servers;
	TcpListeners	tcpListeners;
	if (parse_config_file(av[1], servers))
		return(1);
	// fix last } in configfile.
	if (run_server(servers, tcpListeners))
		return(1);
	return(0);
}
//