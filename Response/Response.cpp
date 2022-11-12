/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/11 13:29:27 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int        Request::methodAllowed()
{
    for(std::vector<std::string>::iterator i = m_location.getMethod().begin();i != m_location.getMethod().end(); i++)
        if (*i == m_method)
            return (0);
    return(1);
}

int     Request::matchLocation()
{
    std::vector<Location> locations = m_server.getLocation();
    for(std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
    {
        std::string sub = m_uri.substr(0,i->getPath().size());
        if (i->getPath() == sub)
        {
            m_location = *i;
            return(0);
        }
    }
    return(1);
}
Server      matchServer(std::vector<Server> servers, std::string headers)
{
    size_t pos = headers.find("Host: ");
    std::string host;
    if (pos != std::string::npos)
    {
        host = headers.substr(pos);
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

Response::~Response(){}
Response::Response(){}

int       Response::handleGet()
{
    if (req)
}

void      Response::setErrorPage()
{
    m_response += m_statusCode;
    m_response += "\n\n";
    if()
    
    //fill error pages
    // switch over status code;
}

Response::Response(Request request, std::vector<Server> servers)
{
    m_response = "HTTP/1.1 ";
    m_server = matchServer(servers, request.getHeaders());
    m_statusCode = request.isWellFormed();
    if (m_statusCode != "200 OK\n")
        setErrorPage();
    if (m_body.length() > m_server.getMaxBodySize())
        return("413 Request Entity Too Large\n");
    if(this->matchLocation())
        return("404 Not Found\n");
    if (m_location.getRedirection().first != "")
        return("301 Moved Permanently\n");
    if (this->methodAllowed())
        return("405 Method Not Allowed\n");
    else if (request.getMethod() == "GET")
        handleGet();
    else if (request.getMethod() == "POST")
        handlePost();
    else if (request.getMethod() == "DELETE")
        handleDelete();
}