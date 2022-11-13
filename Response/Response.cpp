/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/13 22:50:20 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int        Response::methodAllowed(const Request &request)
{
    for(std::vector<std::string>::iterator i = m_location.getMethod().begin();i != m_location.getMethod().end(); i++)
        if (*i == request.getMethod())
            return (0);
    return(1);
}

bool     Response::matchLocation(const Request &request)
{
    std::vector<Location> locations = m_server.getLocation();
    for(std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
    {
        std::string sub = request.getUri().substr(0,i->getPath().size());
        if (i->getPath() == sub)
        {
            m_location = *i;
            break;
        }
    }
    if (m_location.getRedirection().first != "")
    {
        m_statusCode = "301 Moved Permanently\n";
        return(0);
    }
    return(1);
}

bool      Response::matchServer(const std::vector<Server> &servers, const Request &request)
{
    std::string serverName, port, host;
    host = request.getHeader("Host");
    size_t pos = host.find(":");
    if (pos != std::string::npos)
    {
        serverName = host.substr(0, pos);
        port = host.substr(pos + 1);
    }
    bool first = false;
    for(std::vector<Server>::const_iterator i = servers.begin(); i != servers.end(); i++)
    {
        if (i->getPort() == atoi(port.c_str()))
        {
            if (first == false)
            {
                m_server = *i;
                first = true;
            }
            if (i->getName() == serverName)
            {
                m_server = *i;
                break;
            }
        }
    }
	if (request.getHeader("Content_length") > m_server.getMaxBodySize())
    {
        m_statusCode = "413 Request Entity Too Large\n";
        return(false);
    }
    return(true);
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
bool        Response::isWellFormed(const Request &request)
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


    // if (this->methodAllowed(request))
    //     return("405 Method Not Allowed\n");
    else
        return (true);
}

Response::Response(const Request& request, const std::vector<Server> &servers)
{
    m_response = "HTTP/1.1 ";
    if (!isWellFormed(request))
        setErrorPage();
	else if (!matchServer(servers, request))
        setErrorPage();
    else if(!matchLocation(request))
        setErrorPage();
    else if (request.getMethod() == "GET")
        handleGet();
    else if (request.getMethod() == "POST")
        handlePost();
    else if (request.getMethod() == "DELETE")
        handleDelete();
}