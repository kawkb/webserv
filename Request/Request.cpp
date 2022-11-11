/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/10 14:47:21 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"


Request::Request()
{
}

Request::~Request()
{
}

Request::Request(int sd)
{
    m_sd = sd;
    m_serverName = "";
    m_request = "";
    m_bodyStart = 0;
    m_firstLine = 1;
    m_headerStart = 0;
}

int     Request::getSd(void) const{return(m_sd);}
std::string Request::getMethod(void)const{return(m_method);}

Server      Request::matchServer(std::vector<Server> servers)
{
    size_t pos = m_headers.find("Host: ");
    std::string host;
    if (pos != std::string::npos)
    {
        host = m_headers.substr(pos);
        host = host.substr(host.find(" ") + 1, host.find('\n') - host.find(" ") - 1);
        m_serverName = host.substr(0, host.find(':'));
        m_port = host.substr(host.find(':') + 1);
    }
    for(std::vector<Server>::iterator i = servers.begin(); i != servers.end(); i++)
    {
        if (i->getPort() == atoi(m_port.c_str()))
        {
            for(std::vector<Server>::iterator j = i + 1; j != servers.end(); j++)
            {
                if (j->getName() == m_serverName)
                {
                    m_server = *j;
                    return(*j);
                }
            }
                m_server = *i;
                return(m_server);
        }
    }
}

int     Request::matchLocation()
{
    std::vector<Location> locations = m_server.getLocation();
    for(std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
    {
        if (i->getPath() == m_uri)
        {
            m_location = *i;
            return(0);
        }
    }
    return(1);
}

void    Request::parse(const char *buf)
{
    std::string line;
    m_request.append(std::string(buf));
    while(m_request.find("\n") != std::string::npos)
    {
        std::size_t pos = m_request.find("\n");
        line = m_request.substr(0, pos + 1);
        if (m_firstLine == 1)
        {
            std::vector<std::string> tokens = tokenize(line);
            m_firstLine = 0;
            for(std::vector<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
            {
                if(*i == "GET" || *i == "POST" || *i == "DELETE")
                    m_method = *i;
                else if(*i == "HTTP/1.1")
                    m_version = *i;
                else
                    m_uri = *i;
            }
        }
        else if (line.size() == 1)
            m_bodyStart = 1;
        else if (m_bodyStart == 1)
            m_body += line;
        else
        {
            m_headerStart = 1;
            m_headers += line; 
        }
        m_request.erase(0, pos + 1);
    }
    if (m_bodyStart == 1)
    {
        m_body += m_request;
        m_request = "";
    }
    else if (m_headerStart == 1)
    {
        m_headers += m_request; 
        m_request = "";
    }
}

int        Request::methodAllowed()
{
    for(std::vector<std::string>::iterator i = m_location.getMethod().begin();i != m_location.getMethod().end(); i++)
        if (*i == m_method)
            return (0);
    return(1);
}
std::string        Request::isWellFormed(void)
{
    if ((m_method != "GET" && m_method != "POST" && m_method != "DELETE") || m_version != "HTTP/1.1")
        return("400 Bad Request\n");
    if (m_headers.find("Transfer-Encoding") != std::string::npos && m_headers.find("chunked") == std::string::npos)
        return("501 Not Implemented\n");
    if (m_headers.find("Transfer-Encoding") == std::string::npos && m_headers.find("Content-Length") == std::string::npos && m_method == "POST")
        return("400 Bad Request\n");
    if (m_uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
        return("400 Bad Request\n");
    if (m_uri.length() > 2048)
        return("414 Request-URI Too Long\n");
    if (m_body.length() > m_server.getMaxBodySize())
        return("413 Request Entity Too Large\n");
    if(this->matchLocation())
        return("404 Not Found\n");
    if (m_location.getRedirection().first != "")
        return("301 Moved Permanently\n");
    if (this->methodAllowed())
        return("405 Method Not Allowed\n");
    else
        return("200 OK\n");
}
