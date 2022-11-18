/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/18 06:08:58 by kdrissi-         ###   ########.fr       */
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
    m_cursor = 0;
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
    m_cursor = cp.m_cursor;
    m_queryString = cp.m_queryString;
    return (*this);
}

int                                 Request::getSd(void) const{return(m_sd);}
std::string                         Request::getUri(void) const{return(m_uri);}
std::string							Request::getQueryString(void) const {return(m_queryString);}
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
	std::map<std::string, std::string> headers = request.getHeaders();
	// std::FILE *body = request.getBody();
    out << "=============Request==============" << std::endl;
    out << "request sd: " << request.getSd() << std::endl;
    out << "request method: " << request.getMethod()<< std::endl;
    out << "request uri: " << request.getUri()<< std::endl;
    out << "request version: " << request.getVersion()<< std::endl;
    out << "request headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator i = headers.begin(); i != headers.end(); ++i)
	    out << "  " << i->first << ":" << i->second << std::endl;
    out << "request body: " << std::endl;
    // char buffer[252];
    // while (!feof(body))
    // {
    //     fread(buffer, sizeof(buffer), 1, body);
    //     std::cout << buffer << std::endl;
    // }
    // fclose(body);
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
    size_t contentLength = atoi(getHeader("Content-Length").c_str());
    if (getHeader("Transfer-Encoding") == "chunked")
    {
        // std::cout << m_requestBuffer.size() << std::endl ;
        // for (std::vector<char>::iterator i = m_requestBuffer.begin(); i != m_requestBuffer.end(); i++)
        //     std::cout << "|." << *i << "|" << std::endl;
        // for(std::vector<char>::iterator i = m_requestBuffer.begin(); i != m_requestBuffer.end(); i++)
        // {
        //     if(*i == '\r' && (i + 1) != m_requestBuffer.end() && *(i + 1) == '\n')
        //     {
        //         int chunckLen = ;
        //     }
        // }
        // fwrite(m_requestBuffer.data(), sizeof(char), m_requestBuffer.size(), m_body);
        // rewind(m_body);
        // fclose(m_body);
        // return;
    }
    else if (m_bodyLength < contentLength)
    {
        m_requestBuffer.erase(m_requestBuffer.begin(), m_requestBuffer.begin() + m_cursor);
        if (m_requestBuffer.size() > contentLength)
        {
            fwrite(m_requestBuffer.data(), sizeof(char), contentLength - m_bodyLength, m_body);
            m_status = "ok";
            return;
        }
        m_bodyLength = m_requestBuffer.size(); 
        fwrite(m_requestBuffer.data(), sizeof(char), m_requestBuffer.size(), m_body);
        rewind(m_body);
        return;
    }
}

void    Request::parse(const std::vector<Server> &servers, const char *buf, int bufSize)
{
    (void)servers;
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.end(), vectorHugo.begin(),vectorHugo.end());
    if (!m_bodyStart)
    {
        for(std::vector<char>::iterator i = m_requestBuffer.begin() + m_cursor ; i != m_requestBuffer.end(); i++)
        {
            if(*i == '\r' && (i + 1) != m_requestBuffer.end() && *(i + 1) == '\n')
            {
                std::string line = std::string(m_requestBuffer.begin() + m_cursor, i);
                std::cout << line.size() << std::endl;
                if (m_firstLine)
                    fillReqLine(line);
                else if ((i + 2) != m_requestBuffer.end() && *(i + 2) == '\r' && (i + 3) != m_requestBuffer.end() && *(i + 3) == '\n')
                {
                    addHeader(line);
                    std::cout << "hello world" << std::endl;
                    checkErrors(servers);
                }
                else
                    addHeader(line);
                m_cursor += line.size();
                if (m_status != "")
                    return;
            }
            // std::cout << "the party never ends" << std::endl;
        }
    }
    else
        std::cout << "---" << m_requestBuffer[m_cursor] << "---" << std::endl;
        // std::cout << "----" << std::string(m_requestBuffer.begin() + m_cursor , m_requestBuffer.end()) << "----" <<std::endl;
    // else
    //     fillBody();
}
