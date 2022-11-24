/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:30:27 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 09:30:15 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "utils/utils.hpp"
#include "./configFileParser/Server.hpp"
#include "./configFileParser/Location.hpp"
#include "TcpListener/TcpListener.hpp"
#include "TcpListener/Webserv.hpp"
#include "Request/Request.hpp"
#include "Response/Response.hpp"
# define BUFFER_SIZE 1024



int		initiate_master_sockets(std::vector<Server> &server, std::vector<TcpListener> &tcpListener);
void     run_server(std::vector<Server> &servers, std::vector<TcpListener> &tcpListener);
void     parse_config_file(char *av, std::vector<Server> &server);
int     check_server(std::vector<Server> &server);
void	exit_failure(std::string str);
