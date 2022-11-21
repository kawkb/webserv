/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 15:14:24 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/21 00:38:29 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int     main(int ac, char **av)
{
	if (ac != 2)
		exit_failure("Error: Bad Arguments!");
	std::vector<Server>			servers;
	std::vector<TcpListener>	tcpListeners;
	parse_config_file(av[1], servers);
	// fix last } in configfile.
	// run_server(servers, tcpListeners);
	return(0);
}
