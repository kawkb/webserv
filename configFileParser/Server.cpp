/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 20:50:28 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/26 02:54:42 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"
Server::Server(std::ifstream &myfile, size_t &lineCount)
{
    std::string                 line;
    bool		                isServer = 0;
    std::vector<std::string>    token;
    m_port = -1;
    m_autoIndex = 0;
    m_serverName = "";
    m_root = "";
    m_index = "";
    m_maxBodySize = 1000;
    while (getline(myfile, line))
    {
        if ((line.empty() || line.find_first_not_of(" \t") == std::string::npos || line.erase(0, line.find_first_not_of(" \t")).rfind("#", 0) == 0) && lineCount++)
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
                    myfile.close();
                    exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
                }
            }
        }
        else if (token[0] == "error_page" && size == 3 && lineCount++ && isServer)
            m_errorPage.insert(std::pair<std::string, std::string>(token[1], token[2]));
        else if (token[0] == "listen" && size == 2 && lineCount++ && isServer && m_port == -1)
        {
            if(token[1].find_first_not_of("012346789") != std::string::npos)
            {
                myfile.close();
                exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
            }
            else
                m_port =  atoi(token[1].c_str());
        }
        else if (token[0] == "server_name" && size == 2 && lineCount++ && isServer && m_serverName == "")
            m_serverName = token[1];
        else if (token[0] == "root" && size == 2 && lineCount++ && isServer && m_root == "")
        {
            m_root  = token[1];
            if(*m_root.rbegin() != '/')
		        m_root.append("/");
        }
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
                myfile.close();
                exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
            }
        }
        else if (token[0] == "cgi" && size == 3 && lineCount++ && isServer)
		{
			m_cgi[token[1]] = token[2];
		}
        else if (token[0] == "max_body_size" && size == 2 && lineCount++ && isServer && m_maxBodySize == 1000)
        {
            if (token[1].find_first_not_of("0123456789") == std::string::npos)
                m_maxBodySize = atoll(token[1].c_str());
            else
            {
                myfile.close();
                exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
            }  
        }
        else if (token[0] == "location" && size == 2 && lineCount++ && isServer)
        {
			if(m_method.empty())
				m_method.push_back("GET");
            Location    holder;
            holder.setPath(token[1]);
            holder.parse(myfile, lineCount);
			std::vector<std::string> methods = holder.getMethod();
			if (methods.empty() && holder.getGotMethod() == 0)
				for(std::vector<std::string>::iterator it = m_method.begin(); it != m_method.end(); it++)
					holder.setMethod(*it);
            m_location.push_back(holder);
        }
        else if(token[0] == "}" && size == 1 && isServer && lineCount++)
        {
            isServer = 0;
            checkError(myfile);
            return;
        }
        else
        {
            myfile.close();
            exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
        }
    }
    if (isServer)
    {
        myfile.close();
        exit_failure("Configfile Error: missing } in server block");
    }
    checkError(myfile);
}


void	Server::checkError(std::ifstream &myfile)
{
	if (m_port < 0 || m_port > 65536)
    {
        myfile.close();
		exit_failure("mConfig File Error: Port out of range.");
    }
	if(m_serverName == "")
    {
        myfile.close();
		exit_failure("Config File Error: Missing server name.");
    }
	if(m_method.empty())
		m_method.push_back("GET");
    if(m_root == "")
    {
        myfile.close();
		exit_failure("Config File Error: Missing root.");
    }
	if(checkPath(m_root) != "")
    {
        myfile.close();
		exit_failure("Config File Error: "+ checkPath(m_root));
    }
    for(std::map<std::string, std::string>::iterator it = m_cgi.begin(); it != m_cgi.end(); it++)
    {
		struct stat buffer;
		if (stat(it->second.c_str(), &buffer) != 0)
		{
			if (errno == ENOENT)
			{
				myfile.close();
				exit_failure("Config File Error: "+ it->second +" does not exist");
			}
			else if (errno == EACCES)
			{
				myfile.close();
				exit_failure("Config File Error: "+ it->second +" is not accessible");
			}
			else
			{
				myfile.close();
				exit_failure("Config File Error: "+ it->second +" is not a file");
			}
		}
    }
}


std::string   Server::getErrorPage(std::string errorcode) const
{
	std::map<std::string,std::string>::const_iterator i = m_errorPage.find(errorcode);
	if (i != m_errorPage.end())
		return (i->second);
	return ("");
}

std::string						 	Server::getCgiPath(std::string extention) const
{
	std::map<std::string,std::string>::const_iterator i = m_cgi.find(extention);
	if (i != m_cgi.end())
		return (i->second);
	return ("");
}

int                                 Server::getPort(void) const{return(m_port);}
int                                 Server::getAutoIndex(void) const{return(m_autoIndex);}
long long                           Server::getMaxBodySize(void) const{return(m_maxBodySize);}
void                                Server::setMethod(std::string method){m_method.push_back(method);}
std::string                         Server::getName(void) const{return(m_serverName);}
std::string                         Server::getRoot(void) const{return(m_root);}
std::string                         Server::getIndex(void) const{return(m_index);} 
std::vector<std::string>            Server::getMethod(void) const{return(m_method);}
std::vector<Location>               Server::getLocation(void) const{return(m_location);}
std::map<std::string, std::string>  Server::getCgi(void) const{return(m_cgi);}

Server::Server()
{

}

Server::Server(const Server &cp){*this = cp;}

Server & Server::operator=(const Server &cp)
{
	if (this == &cp)
		return (*this);
	m_port = cp.m_port;
	m_autoIndex = cp.m_autoIndex;
	m_serverName = cp.m_serverName;
	m_root = cp.m_root;
	m_index = cp.m_index;
	m_maxBodySize = cp.m_maxBodySize;
	m_method = cp.m_method;
	m_cgi = cp.m_cgi;
	m_location = cp.m_location;
	m_errorPage = cp.m_errorPage;
    return (*this);
}

Server::~Server(){}
