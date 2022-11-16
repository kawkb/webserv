/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/15 15:09:31 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(){}
    
Request::~Request(){}

Request::Request(int sd)
{
    m_sd = sd;
    m_method = "";
    m_uri = "";
    m_version = "";
    m_bodyLength = 0;
    m_firstLine = true;
    m_headerStart = false;
    m_bodyStart = false;
    m_status = "";
    m_queryString = "";
}

Request::Request(const Request &cp)
{
    *this = cp;
}

Request&    Request::operator= (const Request &cp)
{
    m_sd = cp.m_sd;
    m_requestBuffer = cp.m_requestBuffer;
    m_method = cp.m_method;
    m_uri = cp.m_uri;
    m_version = cp.m_version;
    m_server = cp.m_server;
    m_location = cp.m_location;
    m_headers = cp.m_headers;
    m_body = cp.m_body;
    m_bodyLength = cp.m_bodyLength;
    m_firstLine = cp.m_firstLine;
    m_headerStart = cp.m_headerStart;
    m_bodyStart = cp.m_bodyStart;
    m_status = cp.m_status;
    m_queryString = cp.m_queryString;
    return (*this);
}

int                                 Request::getSd(void) const{return(m_sd);}
std::string                         Request::getUri(void) const{return(m_uri);}
std::string                         Request::getVersion(void) const{return(m_version);}
Server                              Request::getServer(void) const{return(m_server);}
Location                            Request::getLocation(void) const{return(m_location);}
std::vector<char>                   Request::getRequestBuffer(void) const{return(m_requestBuffer);}
std::string                         Request::getMethod(void)const{return(m_method);}
std::map<std::string, std::string>  Request::getHeaders(void) const {return(m_headers);}
std::FILE                           *Request::getBody(void) const {return(m_body);}
std::string                         Request::getStatus(void)const {return(m_status);}
std::string                         Request::getError(void) const{return (m_status);}
std::string            				Request::getHeader(std::string key)const
{
	std::map<std::string,std::string>::const_iterator i = m_headers.find(key);
	if (i == m_headers.end())
		return ("");
	else
		return (i->second);
}

std::ostream& operator<<(std::ostream& out, Request request)
{
    // char buf[1];
	std::map<std::string, std::string> headers = request.getHeaders();
	// std::FILE *body = request.getBody();
    out << "=============Request==============" << std::endl;
    out << "request sd: " << request.getSd() << std::endl;
    out << "request method: " << request.getMethod()<< std::endl;
    out << "request uri: " << request.getUri()<< std::endl;
    out << "request version: " << request.getVersion()<< std::endl;
    out << "request headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator i = headers.begin(); i != headers.end(); ++i)
	    out << "  |" << i->first << "|:" << i->second << std::endl;
    out << "request body: " << std::endl;
    // long lSize;
    // char * buffer;
    // size_t result;
    // while (!feof(body))
    // {
    //     fread(&buf, 1, 1, body);
    //     std::cout << buf << std::endl;
    // }
    
    //     fclose(body);
	return out;
}

std::vector<std::string> split(std::string str, std::string sep)
{
	std::vector<std::string> ret;
	std::string token;
	size_t pos = str.find(sep);
	if (pos == std::string::npos)
	{
		ret.push_back(str);
		return ret;
	}
	while ((pos = str.find(sep)) != std::string::npos)
	{
		token = str.substr(0, pos);
		ret.push_back(token);
		str.erase(0, pos + sep.length());
	}
	ret.push_back(str);
	return ret;
}

bool Request::methodAllowed(void)
{
	for (std::vector<std::string>::iterator i = m_location.getMethod().begin(); i != m_location.getMethod().end(); i++)
		if (*i == m_method)
			return (true);
    m_status = "405";
	return (false);
}

bool Request::matchLocation(void)
{
	std::vector<Location> locations = m_server.getLocation();
	for (std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
	{
		std::string sub = m_uri.substr(0, i->getPath().size());
		if (i->getPath() == sub)
		{
			m_location = *i;
			break;
		}
	}
    //get server location;
	if (m_location.getRedirection().first != "")
    {
		m_status = "301 Moved Permanently\n";
        return(false);
    }
    return(true);
}

bool Request::matchServer(const std::vector<Server> &servers)
{
	std::string serverName, port, host;
	host = getHeader("Host");
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
	if (atoi(getHeader("Content_length").c_str()) > m_server.getMaxBodySize())
	{
		m_status = "413";
		return (false);
	}
	return (true);
}

bool   Request::isWellFormed(void)
{
	if ((m_method != "GET" && m_method != "POST" && m_method != "DELETE"))
		m_status = "501";
	if (m_version != "HTTP/1.1")
		m_status = "505";
	if (getHeader("Transfer-Encoding") != "chunked" &&  getHeader("Transfer-Encoding") != "")
		m_status = "501";
	if (m_method == "POST" && getHeader("Transfer-Encoding").empty() && getHeader("Content-Length").empty())
		m_status = "400";
	if (m_uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		m_status = "400";
	if (m_uri.length() > 2048)
		m_status = "414";
    if (m_status != "")
        return (false);
    return(true);
}

void    Request::checkErrors(const std::vector<Server> &servers)
{
    if (!isWellFormed())
        return;
    if (!matchServer(servers))
        return;
    if (!matchLocation())
        return;
    // if (!methodAllowed())
    //     return;
    m_bodyStart = true;
}

bool    Request::fillReqLine(std::string line)
{
    m_firstLine = false;
    std::vector<std::string> tokens = tokenize(line);
    if(tokens.size() == 3)
    {
        m_method = tokens[0];
        m_version = tokens[2];
        const std::string tmp = tokens[1];
		size_t pos = tmp.find("?");
		if (pos != std::string::npos)
		{
			m_uri = tmp.substr(0, pos);
			m_queryString = tmp.substr(pos + 1);
		}
		else
			m_uri = tmp;
		return (true);
    }
    else
    {
        m_status = "400";
        return(false);
    }
}

void    Request::addHeader(std::string line)
{
    m_headerStart = true;
    size_t found = line.find(':');
    if(found != std::string::npos)
        m_headers.insert(std::pair<std::string, std::string>(line.substr(0, found), line.substr(found + 2)));
}

void    Request::fillBody()
{
    m_body = tmpfile();
    // char buffer [256];
    // std::cout << m_bodyLength << "==="<< m_requestBuffer.size()<< std::endl;
    // std::cout <<getHeader("Content-Length") << std::endl;
    // std::cout << m_bodyLength << "==="<<atoi(getHeader("Content-Length").c_str())<< std::endl;
    // if ((int)m_bodyLength < atoi(getHeader("Content-Length").c_str()))
    // {
    //     if (m_requestBuffer.size() > m_bodyLength)
    //     {
    //         m_requestBuffer.erase(m_requestBuffer.begin() + m_bodyLength, m_requestBuffer.end());
    //         // fputs(reinterpret_cast<char*> (&m_requestBuffer[0]), m_body);
    //         std::cout<<reinterpret_cast<char*> (&m_requestBuffer[0]) << std::endl;
    //         m_status = "ok";
    //         return;
    //     }
                    std::cout <<"|"<<std::string(m_requestBuffer.begin(),m_requestBuffer.end()) <<"|"<< std::endl;

        // m_bodyLength = m_requestBuffer.size();
        // std::cout << reinterpret_cast<char*> (&m_requestBuffer[0])  << "test"<< std::endl;
        // fputs(reinterpret_cast<char*> (&m_requestBuffer[0]), m_body);
        // while (!feof(m_body)) 
        // {
        // if (fgets (buffer, 256, m_body) == NULL) break;
        // fputs (buffer,stdout);
        // }
        
        // return;
    // }
    // else if (getHeader("Transfer-Encoding") == "chunked")
    // {
        
    // }
}

void    Request::parse(const std::vector<Server> &servers, const char *buf, int bufSize)
{
    (void)servers;
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.end(), vectorHugo.begin(),vectorHugo.end());
    std::vector<char>::iterator pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\r'); 
    while(pos != m_requestBuffer.end() && (pos + 1) != m_requestBuffer.end() && *(pos + 1) == '\n' && m_bodyStart == false)
    {
        std::string line =  std::string(m_requestBuffer.begin(), pos);
        if (m_firstLine)
            fillReqLine(line);
        else if ((pos + 2) != m_requestBuffer.end() && *(pos + 2) == '\r')
        {
            addHeader(line);
            checkErrors(servers); // check /r /n
        }
        else
            addHeader(line);
        if (m_status != "")
            return;
        m_requestBuffer.erase(m_requestBuffer.begin(), pos + 2);
        pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\r');
    }
    if (m_bodyStart)
        fillBody();
    if (m_headerStart)
    {
        addHeader(std::string(m_requestBuffer.begin(), m_requestBuffer.end()));
        m_requestBuffer.clear();
    }
}
