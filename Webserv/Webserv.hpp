/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 14:25:14 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/21 19:42:39 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Config class with pointer to servers default constructor and deconsructor
#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../Server/Server.hpp"

#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

# define MAX_FDS 1024

class Webserv {
    public:
        Webserv();
        Webserv(std::string config_file);
        ~Webserv();
        void run();
        void debug();
        void attach(const Server server);
        void save_location(Server *server, std::istream &config_file_stream, std::string line);
        void save_server(std::istream &config_file_stream, std::string line);

    private:
        std::string config_file;
        std::vector<Server> servers;
};

#endif