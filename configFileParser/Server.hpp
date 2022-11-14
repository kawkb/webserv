/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 11:01:45 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/09 16:22:05 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
#include <stdlib.h>
#include "Location.hpp"

class Server
{
	private:
		int                                 m_port;
		int                                 m_autoIndex;
		std::string                         m_serverName;
		std::string                         m_root;
		std::string                         m_index;
		std::string                         m_cgi;
		std::vector<Location>               m_location;
		std::map<std::string, std::string>  m_errorPage;
		std::vector<std::string>            m_method;
		int                                 m_maxBodySize;
	public:
											Server();
											Server(const Server &cp);
											~Server();
		int                                 parse(std::ifstream &myfile);
		std::string                         getName(void) const;
		int                                 getPort(void) const;
		std::vector<std::string>            getMethod(void) const;
		std::string                         getRoot(void) const;
		std::string                         getIndex(void) const;
		int                                 getAutoIndex(void) const;
		std::string                         getCgi(void) const;
		int                                 getMaxBodySize(void) const;
		std::vector<Location>               getLocation(void) const;
		std::string  						getErrorPage(std::string errorcode) const;
		Server& 							operator=(const Server &cp);
};

std::ostream& operator<<(std::ostream& os, Server server);
