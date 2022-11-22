/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 15:14:24 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/22 05:19:48 by moerradi         ###   ########.fr       */
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
	run_server(servers, tcpListeners);
	return(0);
}
