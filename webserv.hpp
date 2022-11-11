/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:30:27 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/06 21:32:37 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "configFileParser/Server.hpp"
#include "TcpListener/TcpListener.hpp"
#include "Request/Request.hpp"


int		initiate_master_sockets(std::vector<Server> &server, std::vector<TcpListener> &tcpListener);
int     run_server(std::vector<Server> &servers, std::vector<TcpListener> &tcpListeners);
int     parse_config_file(char *av, std::vector<Server> &server);
int     check_server(std::vector<Server> &server);
void	exit_failure(std::string str);
