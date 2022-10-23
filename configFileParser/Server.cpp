/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:50:28 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/09/25 13:44:21 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string>    tokenize(std::string line)
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
        tokens.push_back(line);
    return(tokens);
}

int     Server::parse(std::ifstream &myfile)
{
    std::string line;
    std::vector<std::string> token;
    while (getline(myfile, line))
    {
        if (!line.empty() && line.find_first_not_of(" \t") != std::string::npos)
            token = tokenize(line);
        int size = token.size();
        if (line.empty() || line.find_first_not_of(" \t") == std::string::npos || token[0] == "{")
            continue;
        else if (token[0] == "allow_methods" && size <= 4)
        {
            for( int i = 1; i < size; i++)
            {  
                if (token[i] == "GET" || token[i] == "POST" || token[i] == "DELETE")
                    m_method.push_back(token[i]);
                else
                {
                    std::cout << "\033[1;31mConfigfile error: \033[0m" << line << std::endl;
                    return(1);
                }
            }
        }
        else if (token[0] == "error_page" && size == 3)
            m_errorPage.insert(std::pair<std::string, std::string>(token[1], token[2]));
        else if (token[0] == "listen" && size == 2 )
        {
            if (token[1].find_first_not_of("0123456789") == std::string::npos)
                m_port = atoi(token[1].c_str());
            if (m_port == 0)
            {
                std::cout << "\033[1;31mConfigfile error: \033[0m" << line << std::endl;
                return(1);
            }   
        }
        else if (token[0] == "server_name" && size == 2 )
            m_serverName = token[1];
        else if (token[0] == "root" && size == 2 )
            m_root  = token[1];
        else if (token[0] == "index" && size == 2 )
            m_index = token[1];
		else if (token[0] == "autoindex" && size == 2 )
            m_autoIndex = token[1] == "on" ? 1 : 0;
        else if (token[0] == "cgi"&& size == 2 )
            m_cgi = token[1];
        else if (token[0] == "max_body_size" && size == 2 )
        {
            if (token[1].find_first_not_of("0123456789") == std::string::npos)
                m_maxBodySize = atoi(token[1].c_str());
            else
            {
                std::cout << "\033[1;31mConfigfile error: \033[0m" << line << std::endl;
                return(1);
            }  
        }
        else if (token[0] == "location" && size == 2 )
        {
            Location holder;
            holder.setPath(token[1]);
            if (holder.parse(myfile))
                return(1);
            m_location.push_back(holder);
        }
        else if (token[0] != "}")
        {
            std::cout << "\033[1;31mConfigfile error: \033[0m" << line << std::endl;
            return(1);
        }
        else
            return(0);
    }
    return (0);
}

std::ostream& operator<<(std::ostream& out, Server server)
{
    out << "=============Server==============" << std::endl;
    std::vector<std::string> method = server.getMethod();
    std::vector<Location> location = server.getLocation();
    std::map<std::string, std::string>  errorPage = server.getErrorPage();
    std::map<std::string, std::string >::iterator itr;
	out << "Server port: " << server.getPort() << std::endl;
    out << "Server name: " << server.getName() << std::endl;
    out << "server method: ";
	for (long unsigned int i = 0; i < method.size(); i++)
	    out << method[i] << " ";
    out << std::endl;
	out << "server root:" << server.getRoot() << std::endl;
	out << "server index:" << server.getIndex() << std::endl;
	out << "server autoIndex:" << server.getAutoIndex() << std::endl;
    out << "server bodysize: " << server.getMaxBodySize() << std::endl;
    for( itr = errorPage.begin(); itr != errorPage.end();++itr)
        out << "server error page: " << itr->first << " " << itr->second << std::endl;
    // for (long unsigned int i = 0; i < location.size(); i++)
    // {
    //     out << location[i];
    // }
	return out;
}

std::map<std::string, std::string>    Server::getErrorPage(void) const
{
    return(m_errorPage);
}
std::vector<Location> Server::getLocation(void) const
{
    return(m_location);
}

int     Server::getPort(void) const
{
    return(m_port);
}
std::string     Server::getName(void) const
{
    return(m_serverName);
}
std::vector<std::string>     Server::getMethod(void) const
{
    return(m_method);
}
std::string     Server::getRoot(void) const
{
    return(m_root);
}
std::string     Server::getIndex(void) const
{
    return(m_index);
} 
int     Server::getAutoIndex(void) const
{
    return(m_autoIndex);
}

std::string Server::getCgi(void) const
{
    return(m_cgi);
}

int     Server::getMaxBodySize(void) const
{
    return(m_maxBodySize);
}

Server::Server()
{
    m_port = -1;
    m_autoIndex = 1;
    m_serverName = "";
    m_root = "";
    m_index = "";
    m_cgi = "";
    m_maxBodySize = 1000;
}

Server::Server(const Server &cp)
{
    *this = cp;
}

Server & Server::operator=(const Server &cp)
{
    m_port = cp.getPort();
    m_autoIndex = cp.getAutoIndex();
    m_serverName = cp.getName();
    m_root = cp.getRoot();
    m_index = cp.getIndex();
    m_cgi = cp.getCgi();
    m_location = cp.getLocation();
    m_method = cp.getMethod();
    m_maxBodySize = cp.getMaxBodySize();
    m_errorPage = cp.getErrorPage();
    return (*this);
}

Server::~Server()
{
}
//smaller code
//start tcplistner shit
