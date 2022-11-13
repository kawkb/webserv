/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/13 18:33:37 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int        Response::methodAllowed(Request request)
{
    for(std::vector<std::string>::iterator i = m_location.getMethod().begin();i != m_location.getMethod().end(); i++)
        if (*i == request.getMethod())
            return (0);
    return(1);
}

int     Response::matchLocation(Request request)
{
    std::vector<Location> locations = m_server.getLocation();
    for(std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
    {
        std::string sub = request.getUri().substr(0,i->getPath().size());
        if (i->getPath() == sub)
        {
            m_location = *i;
            return(0);
        }
    }
    return(1);
}

bool      Response::matchServer(std::vector<Server> servers, std::string host)
{
    std::string serverName, port;
    size_t pos = host.find("Host: ");
    if (pos != std::string::npos)
    {
        host = host.substr(pos);
        host = host.substr(host.find(" ") + 1, host.find('\n') - host.find(" ") - 1);
        serverName = host.substr(0, host.find(':'));
        port = host.substr(host.find(':') + 1);
    }
    for(std::vector<Server>::iterator i = servers.begin(); i != servers.end(); i++)
    {
        if (i->getPort() == atoi(port.c_str()))
        {
            for(std::vector<Server>::iterator j = i + 1; j != servers.end(); j++)
            {
                if (j->getName() == serverName)
                {
                    m_server = *j;
                    return(*j);
                }
            }
                m_server = *i;
                return(m_server);
        }
    }
		if (request.getHeader("Content_length") > m_server.getMaxBodySize())
        return("413 Request Entity Too Large\n");
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
bool        Response::isWellFormed(Request request)
{
    if ((request.getMethod() != "GET" && request.getMethod() != "POST" && request.getMethod() != "DELETE") || request.getVersion() != "HTTP/1.1")
    {
		m_statusCode = "501 Not Implemented\n";
		return (false);
	}
    if (request.getHeader("Transfer-Encoding") != "chunked")
    {
		m_statusCode = "501 Not Implemented\n";
		return (false);
	}
    if (request.getMethod() == "POST" && request.getHeader("Transfer-Encoding").empty() && request.getHeader("Content-Length").empty())
    {
		m_statusCode = "400 Bad Request\n";
		return (false);
	}
    if (request.getUri().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
    {
		m_statusCode = "400 Bad Request\n";
		return (false);
	}
    if (request.getUri().length() > 2048)
    {
		m_statusCode = "414 Request-URI Too Long\n";
		return (false);
	}

    // if(this->matchLocation(request))
    //     return("404 Not Found\n");
    // if (m_location.getRedirection().first != "")
    //     return("301 Moved Permanently\n");
    // if (this->methodAllowed(request))
    //     return("405 Method Not Allowed\n");
    else
        return (true);
}

Response::Response(Request request, std::vector<Server> servers)
{
    m_response = "HTTP/1.1 ";
    matchServer(servers, request.getHeader("host"));
    if (!isWellFormed(request))
        setErrorPage();
	else if (!matchServer(servers, request.getHeader("host")))
        setErrorPage();
    else if (request.getMethod() == "GET")
        handleGet();
    else if (request.getMethod() == "POST")
        handlePost();
    else if (request.getMethod() == "DELETE")
        handleDelete();
}