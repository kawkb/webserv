/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:50:28 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/21 06:16:54 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int     Server::parse(std::ifstream &myfile, size_t &lineCount)
{
    std::string                 line;
    Location                    holder;
    bool		                isServer = 0;
    std::vector<std::string>    token;
    while (getline(myfile, line))
    {
        if ((line.empty() || line.find_first_not_of(" \t") == std::string::npos) && lineCount++)
            continue;
        else
            token = tokenize(line);
        int size = token.size();
        if (token[0] == "{" && size == 1 && !isServer && lineCount++)
			isServer = 1;
        else if (token[0] == "allow_methods" && size <= 4 && lineCount++ && isServer && m_method.empty())
        {
            for( int i = 1; i < size; i++)
            {  
                if (token[i] == "GET" || token[i] == "POST" || token[i] == "DELETE")
                    m_method.push_back(token[i]);
                else
                {
                    std::cerr << "\033[1;31mSERVER Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
                    return(1);
                }
            }
        }
        else if (token[0] == "error_page" && size == 3 && lineCount++ && isServer)
            m_errorPage.insert(std::pair<std::string, std::string>(token[1], token[2]));
        else if (token[0] == "listen" && size == 2 && lineCount++ && isServer && m_port == -1)
            m_port =  atoi(token[1].c_str());
        else if (token[0] == "server_name" && size == 2 && lineCount++ && isServer && m_serverName == "localhost")
            m_serverName = token[1];
        else if (token[0] == "root" && size == 2 && lineCount++ && isServer && m_root == "")
            m_root  = token[1];
        else if (token[0] == "index" && size == 2 && lineCount++ && isServer && m_index == "")
            m_index = token[1];
		else if (token[0] == "autoindex" && size == 2 && lineCount++ && isServer && m_autoIndex == 0)
        {
            if (token[1] == "on")
                m_autoIndex = 1;
            else if (token[1] == "off")
                m_autoIndex = 0;
            else
            {
                std::cerr << "\033[1;31mSERVER Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
                return(1);
            }
            
        }
        else if (token[0] == "cgi" && size == 3 && lineCount++ && isServer && m_cgiExtension == "")
        {
            m_cgiExtension = token[1];
            m_cgiPath = token[2];
        }
        else if (token[0] == "max_body_size" && size == 2 && lineCount++ && isServer && m_maxBodySize == 1000)
        {
            if (token[1].find_first_not_of("0123456789") == std::string::npos)
                m_maxBodySize = atoi(token[1].c_str());
            else
            {
                std::cerr << "\033[1;31mSERVER Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
                return(1);
            }  
        }
        else if (token[0] == "location" && size == 2 && lineCount++ && isServer)
        {
            holder.setPath(token[1]);
            if (holder.parse(myfile, lineCount))
                return(1);
            m_location.push_back(holder);
        }
        else if(token[0] == "}" && size == 1 && isServer && lineCount++ && isServer)
            return(0);
        else
        {
            std::cerr << "\033[1;31mSERVER Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
            return(1);
        }
    }
    std::cerr << "\033[1;31mSERVER Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
    return (1);
}

std::ostream& operator<<(std::ostream& out, Server server)
{
    out << "=============Server==============" << std::endl;
    std::vector<std::string> method = server.getMethod();
    std::vector<Location> location = server.getLocation();
    // std::map<std::string, std::string>  errorPage = server.getErrorPage();
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
    // for( itr = errorPage.begin(); itr != errorPage.end();++itr)
    //     out << "server error page: " << itr->first << " " << itr->second << std::endl;
    // for (long unsigned int i = 0; i < location.size(); i++)
    // {
    //     out << location[i];
    // }
	return out;
}

std::string   Server::getErrorPage(std::string errorcode) const
{
	std::map<std::string,std::string>::const_iterator i = m_errorPage.find(errorcode);
	if (i != m_errorPage.end())
		return (i->second);
	return ("");
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

std::string Server::getCgiExtention(void) const
{
    return(m_cgiExtension);
}

std::string Server::getCgiPath(void) const
{
	return (m_cgiPath);
}

int     Server::getMaxBodySize(void) const
{
    return(m_maxBodySize);
}

Server::Server()
{
    m_port = -1;
    m_autoIndex = 0;
    m_serverName = "localhost";
    m_root = "";
    m_index = "";
    m_cgiExtension = "";
	m_cgiPath = "";
    m_maxBodySize = 1000;
}

Server::Server(const Server &cp)
{
    *this = cp;
}

Server & Server::operator=(const Server &cp)
{
	if (this == &cp)
		return (*this);
	m_port = cp.m_port;
	m_autoIndex = cp.m_autoIndex;
	m_serverName = cp.m_serverName;
	m_root = cp.m_root;
	m_index = cp.m_index;
	m_cgiExtension = cp.m_cgiExtension;
	m_cgiPath = cp.m_cgiPath;
	m_maxBodySize = cp.m_maxBodySize;
	m_method = cp.m_method;
	m_location = cp.m_location;
	m_errorPage = cp.m_errorPage;
    return (*this);
}

Server::~Server()
{
}
//smaller code
//start tcplistner shit
