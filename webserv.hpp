/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:30:27 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/19 03:12:42 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include "configFileParser/Server.hpp"
#include "TcpListener/TcpListener.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
# define BUFFER_SIZE 1024
#typedef std::vector<Server> Servers;
#typedef TcpListeners TcpListeners;


int		initiate_master_sockets(Servers &server, TcpListeners &tcpListener);
int     run_server(Servers &servers, TcpListeners &tcpListeners);
int     parse_config_file(char *av, Servers &server);
int     check_server(Servers &server);
void	exit_failure(std::string str);
