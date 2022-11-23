/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 11:01:45 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/23 02:06:07 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"

class Server
{
	private:
		int                                 m_port;
		int                                 m_autoIndex;
		std::string                         m_serverName;
		std::string                         m_root;
		std::string                         m_index;
		std::string                         m_cgiPath;
		std::string							m_cgiExtension;
		std::vector<Location>               m_location;
		std::map<std::string, std::string>  m_errorPage;
		std::vector<std::string>            m_method;
		int                                 m_maxBodySize;
	public:
											Server();
											Server(const Server &cp);
											~Server();
		int                                 parse(std::ifstream &myfile, size_t &lineCount);
		std::string                         getName(void) const;
		int                                 getPort(void) const;
		std::vector<std::string>            getMethod(void) const;
		std::string                         getRoot(void) const;
		std::string                         getIndex(void) const;
		int                                 getAutoIndex(void) const;
		std::string							getCgiPath(void) const;
		std::string							getCgiExtention(void) const;
		int                                 getMaxBodySize(void) const;
		std::vector<Location>               getLocation(void) const;
		std::string  						getErrorPage(std::string errorcode) const;
		void                        		setMethod(std::string method);
		Server& 							operator=(const Server &cp);
};

std::ostream& operator<<(std::ostream& os, Server server);
