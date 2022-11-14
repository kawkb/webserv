/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/14 17:36:22 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){}
    
Request::~Request(){}

Request::Request(int sd)
{
    m_method = "";
    m_sd = sd;
    m_bodyStart = 0;
    m_firstLine = 1;
    m_headerStart = 0;
    m_status = "200 OK";
}

Request::Request(const Request &cp)
{
    *this = cp;
}

Request&    Request::operator= (const Request &cp)
{
    m_sd = cp.getSd();
    m_requestBuffer = cp.getRequestBuffer();
    m_method = cp.getMethod();
    m_uri = cp.getUri();
    m_version = cp.getVersion();
    m_headers = cp.getHeaders();
    m_body = cp.getBody();
    m_firstLine = cp.getFirstLine();
    m_headerStart = cp.getHeaderStart();
    m_bodyStart = cp.getBodyStart();
    return (*this);
}

void    Request::addHeader(std::string line)
{
    m_headerStart = 1;
    size_t found = line.find(':');
    if(found != std::string::npos)
        m_headers.insert(std::pair<std::string, std::string>(line.substr(0, found), line.substr(found + 2)));
}

void    Request::fillReqLine(std::string line)
{
    m_firstLine = 0;
    std::vector<std::string> tokens = tokenize(line);
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
void    Request::fillBody()
{
                // m_body.insert(m_body.end(), m_requestBuffer.begin(), pos + 1);
                
    std::cout << "here is body" << std::endl;
}

void    Request::isWellformed()
{
    m_bodyStart = 1;
    bool Response::isWellFormed(const Request &request)
    {
	    if ((m_method != "GET" && m_method != "POST" && m_method != "DELETE"))
	    	m_status = "501";
	    if (m_version != "HTTP/1.1")
	    	m_status = "505";
	    if (getHeader("Transfer-Encoding") != "chunked")
	    	m_status = "501";
	    if (m_method == "POST" && getHeader("Transfer-Encoding").empty() && getHeader("Content-Length").empty())
	    	m_status = "400";
	    if (m_uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
	    	m_status = "400";
	    if (m_uri.length() > 2048)
	    	m_status = "414";
    }
}

int Request::methodAllowed(const Request &request)
{
	for (std::vector<std::string>::iterator i = m_location.getMethod().begin(); i != m_location.getMethod().end(); i++)
		if (*i == request.getMethod())
			return (0);
	return (1);
}

bool Request::matchLocation(const Request &request)
{
	std::vector<Location> locations = m_server.getLocation();
	for (std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
	{
		std::string sub = request.getUri().substr(0, i->getPath().size());
		if (i->getPath() == sub)
		{
			m_location = *i;
			return (0);
		}
	}
	return (1);

	if (m_location.getRedirection().first != "")
		return ("301 Moved Permanently\n");
}
bool Request::matchServer(const std::vector<Server> &servers, const Request &request)
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
	for (std::vector<Server>::const_iterator i = servers.begin(); i != servers.end(); i++)
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
		m_statusCode = 413;
		return (false);
	}
	return (true);
}

void    Request::checkErrors()
{
    isWellFormed();
    matchServer();
    matchLocation();
}

void    Request::parse(const char *buf, int bufSize)
{
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.end(), vectorHugo.begin(),vectorHugo.end());
    std::vector<char>::iterator pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\r');
    while(pos != m_requestBuffer.end() && (pos + 1) != m_requestBuffer.end() && *(pos + 1) == '\n')
    {
        std::string line =  std::string(m_requestBuffer.begin(), pos);
        if (m_firstLine == 1)
            fillReqLine(line);
        else if ((pos + 2) != m_requestBuffer.end() && *(pos + 2) == '\r')
            checkErrors();
        else if (m_bodyStart == 1 && m_status == "200 OK")
            fillBody(); 
        else
            addHeader(line);
        m_requestBuffer.erase(m_requestBuffer.begin(), pos + 2);
        pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\r');
    }
    if (m_bodyStart == 1)
    {
        m_body.insert(m_body.end(), m_requestBuffer.begin(), m_requestBuffer.end());
        m_requestBuffer.clear();
    }
    else if (m_headerStart == 1)
    {
        addHeader(std::string(m_requestBuffer.begin(), m_requestBuffer.end()));
        m_requestBuffer.clear();
    }
}

std::ostream& operator<<(std::ostream& out, Request request)
{
	std::map<std::string, std::string> headers = request.getHeaders();
	std::vector<char> body = request.getBody();
    out << "=============Request==============" << std::endl;
    out << "request sd: " << request.getSd() << std::endl;
    out << "request method: " << request.getMethod()<< std::endl;
    out << "request uri: " << request.getUri()<< std::endl;
    out << "request version: " << request.getVersion()<< std::endl;
    out << "request headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator i = headers.begin(); i != headers.end(); ++i)
	    out << "  " << i->first << ":" << i->second << std::endl;
    out << "request body: " << std::endl << std::string(body.begin(), body.end()) << std::endl;
	return out;
}

int                                 Request::getSd(void) const{return(m_sd);}
std::string                         Request::getUri(void) const{return(m_uri);}
std::string                         Request::getVersion(void) const{return(m_version);}
std::vector<char>                   Request::getRequestBuffer(void) const{return(m_requestBuffer);}
std::string                         Request::getMethod(void)const{return(m_method);}
std::map<std::string, std::string>  Request::getHeaders(void) const {return(m_headers);}
std::vector<char>                   Request::getBody(void) const {return(m_body);}
int                                 Request::getFirstLine(void) const{return(m_firstLine);}
int                                 Request::getHeaderStart(void) const{return(m_headerStart);}
int                                 Request::getBodyStart(void) const{return(m_bodyStart);}
std::string                         Request::getHeader(std::string key)const
{
	std::map<std::string,std::string>::const_iterator i = m_headers.find(key);
	if (i == m_headers.end())
		return ("");
	else
		return (i->second);
}