/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:30:27 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/17 22:11:47 by moerradi         ###   ########.fr       */
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


int		initiate_master_sockets(std::vector<Server> &server, std::vector<TcpListener> &tcpListener);
int     run_server(std::vector<Server> &servers, std::vector<TcpListener> &tcpListeners);
int     parse_config_file(char *av, std::vector<Server> &server);
int     check_server(std::vector<Server> &server);
void	exit_failure(std::string str);
