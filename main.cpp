/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 15:14:24 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/13 17:27:37 by kdrissi-         ###   ########.fr       */
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
	std::vector<Server>			servers;
	std::vector<TcpListener>	tcpListeners;
	if (parse_config_file(av[1], servers))
		return(1);
	// fix last } in configfile.
	// add cgi
	if (run_server(servers, tcpListeners))
		return(1);
	return(0);
}