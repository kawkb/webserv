/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 11:01:45 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/25 23:26:08 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"
class Location;
class Server
{
	private:
		int                                 m_port;
		int                                 m_autoIndex;
		std::string                         m_serverName;
		std::string                         m_root;
		std::string                         m_index;
		std::map<std::string, std::string>	m_cgi;
		std::vector<Location>               m_location;
		std::map<std::string, std::string>  m_errorPage;
		std::vector<std::string>            m_method;
		long long                           m_maxBodySize;
	public:
											Server();
											Server(const Server &cp);
											Server(std::ifstream &myfile, size_t &lineCount);
											~Server();
		// getters:
		std::string                         getName(void) const;
		int                                 getPort(void) const;
		std::vector<std::string>            getMethod(void) const;
		std::string                         getRoot(void) const;
		std::string                         getIndex(void) const;
		int                                 getAutoIndex(void) const;
		std::string						 	getCgiPath(std::string extention) const;

		long long                           getMaxBodySize(void) const;
		std::vector<Location>               getLocation(void) const;
		std::string  						getErrorPage(std::string errorcode) const;
		std::map<std::string, std::string>  getCgi(void) const;
		void                        		setMethod(std::string method);
		// methods:
		void								checkError(std::ifstream &myfile);
		Server& 							operator=(const Server &cp);
};

std::ostream& operator<<(std::ostream& os, Server server);
