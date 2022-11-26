/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/25 23:29:37 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

Request::Request(){}
    
Request::~Request(){

}

Request::Request(int sd)
{
    m_filePath = "";
    m_body = NULL;
    m_sd = sd;
    m_method = "";
    m_uri = "";
    m_version = "";
    m_bodyLength = 0;
    m_firstLine = true;
    m_bodyStart = false;
    m_status = "";
    m_chunckLen = 0;
    m_queryString = "";
	m_bodyfd = -1;
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
    m_bodyStart = cp.m_bodyStart;
    m_status = cp.m_status;
    m_queryString = cp.m_queryString;
    m_chunckLen = cp.m_chunckLen;
    m_filePath = cp.m_filePath;
	m_bodyfd = cp.m_bodyfd;
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
std::map<std::string, std::string>  Request::getHeaders(void) const{return(m_headers);}
std::FILE                           *Request::getBody(void) const{return(m_body);}
std::string                         Request::getStatus(void)const{return(m_status);}
std::string                         Request::getError(void) const{return (m_status);}
std::string                         Request::getFilePath(void) const{return(m_filePath);}
std::string            				Request::getHeader(std::string key)const
{
	std::map<std::string,std::string>::const_iterator i = m_headers.find(key);
	if (i == m_headers.end())
		return ("");
	else
		return (i->second);
}
int									Request::getBodyfd(void) const {return(m_bodyfd);}

bool Request::methodAllowed(void)
{
    std::vector<std::string> allowedMethods = m_location.getMethod();
	for (std::vector<std::string>::iterator i = allowedMethods.begin(); i != allowedMethods.end(); i++)
    {
		if (*i == m_method)
			return (true);
    }
    m_status = "405";
	return (false);
}

void Request::matchLocation(void)
{
	// std::string referer = getHeader("Referer");
	// std::string refererPath = "";
	// if (referer != "")
	// {
	// 	refererPath = referer.substr(referer.find("://") + 3);
	// 	refererPath = refererPath.substr(refererPath.find("/"));
	// 	std::cout << "refererPath: " << refererPath << std::endl;
	// }
	std::vector<Location> locations = m_server.getLocation();
	for (std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
	{
		std::string sub;
		std::string locationPath = i->getPath();
		sub = m_uri.substr(0, locationPath.length());
		if (i->getPath() == sub)
		{
			std::string current_path = m_location.getPath();
			if (locationPath.length() > current_path.length())
				m_location = *i;
		}
	}
	std::string locationPath = m_location.getPath();
	if (locationPath.empty())
    	m_location = Location(m_server);
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
	long long conlen = atoll(m_headers["Content-Length"].c_str());
	std::cout << "conlen: " << conlen << std::endl;
	if (conlen > m_server.getMaxBodySize())
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
	if (m_uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:-/?#[]@!$&'()*+,;=%") != std::string::npos)
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
    matchLocation();
    if (!methodAllowed())
        return;
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
    size_t found = line.find(':');
    if(found != std::string::npos)
	{
		std::string key = line.substr(0, found);
		if (key.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-") != std::string::npos)
		{
			m_status = "400";
			return;
		}
		std::string value = line.substr(found + 2);
        m_headers.insert(std::pair<std::string, std::string>(key, value));
	}
}

void    Request::fillChunked(void)
{
	try {
		std::vector<char> temp;
		for(std::vector<char>::iterator i = m_requestBuffer.begin(); i != m_requestBuffer.end(); i++)
		{
			if (m_chunckLen > 0)
			{
				fd_set set;
				FD_ZERO(&set);
				FD_SET(m_bodyfd, &set);
				select(m_bodyfd + 1, &set, NULL, NULL, NULL);
				if (FD_ISSET(m_bodyfd, &set))
				{
					fwrite(&(*i), sizeof(char), 1, m_body);
					m_chunckLen--;
				}
			}
			else if(*i == '\r' && (i + 1) != m_requestBuffer.end() && *(i + 1) == '\n')
			{
				if (!temp.empty())
				{
					m_chunckLen = std::stoul(std::string(temp.begin(), temp.end()), nullptr, 16);
					if (m_chunckLen == 0)
					{
						m_status = "200";
						rewind(m_body);
						return;
					}
					temp.clear(); 
				}
				i++;
			}
			else
				temp.push_back(*i);
		} 
		m_requestBuffer = temp;
	}
	catch(const std::exception& e)
	{
		m_status = "400";
	}
}

void    Request::fillContentLength(size_t contentLength)
{
	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(m_bodyfd, &writefds);
	select(m_bodyfd + 1, NULL, &writefds, NULL, NULL);
    if (m_requestBuffer.size() >= contentLength)
    {
		if (FD_ISSET(m_bodyfd, &writefds))
		{
			fwrite(m_requestBuffer.data(), sizeof(char), contentLength - m_bodyLength, m_body);
			rewind(m_body);
			m_status = "200";
			return;
		}
    }
    m_bodyLength += m_requestBuffer.size();
	if (FD_ISSET(fileno(m_body), &writefds))
    	fwrite(m_requestBuffer.data(), 1, m_requestBuffer.size(), m_body);
    m_requestBuffer.clear();
    if (m_bodyLength >= contentLength)
    {
        rewind(m_body);
        m_status = "200";
    }
    return;
}

void    Request::fillBody(void)
{
    size_t contentLength = atoi(getHeader("Content-Length").c_str());
    if (getHeader("Transfer-Encoding") == "chunked")
        fillChunked();
	else if (contentLength == 0)
		m_status = "400";
	else if (contentLength < 0)
		m_status = "400";
    else if (m_bodyLength <= contentLength)
        fillContentLength(contentLength);
	
}

void    Request::parse(const std::vector<Server> &servers, const char *buf, int bufSize)
{
    char crlf[] = {'\r', '\n', '\r', '\n'};
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.end(), vectorHugo.begin(),vectorHugo.end());
    if (!m_bodyStart)
    {
        std::vector<char>::iterator found = std::search(m_requestBuffer.begin(), m_requestBuffer.end(), crlf, crlf + 4);
        if (found != m_requestBuffer.end())
        {
            int cursor = 0;
            for(std::vector<char>::iterator i = m_requestBuffer.begin(); i != found + 2; i++)
            {
                if (*i == '\r' && *(i + 1) == '\n')
                {
                    std::string line = std::string(m_requestBuffer.begin() + cursor, i);
                    if (m_firstLine)
                        fillReqLine(line);
                    else
                        addHeader(line);
                    cursor += line.size() + 2;
                }
            }
            checkErrors(servers);
            m_requestBuffer.erase(m_requestBuffer.begin(), found + 4);
        }
    }
    if (m_method == "POST" && m_bodyStart)
	{
		if (m_body == NULL)
		{
			m_body = createTmpFile(m_filePath);
			m_bodyfd = fileno(m_body);
		}
        fillBody();
	}
	else if (m_bodyStart && m_status == "")
		m_status = "200";
}
