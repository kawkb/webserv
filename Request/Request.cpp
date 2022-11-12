/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/12 22:41:17 by kdrissi-         ###   ########.fr       */
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
    m_headers.insert(cp.m_headers.begin(), cp.m_headers.end());
    m_body.assign(cp.m_body.begin(), cp.m_body.end());
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
        m_headers.insert(std::pair<std::string, std::string>(line.substr(0, found), line.substr(found + 1)));
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

void    Request::parse(const char *buf, int bufSize)
{
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.end(), vectorHugo.begin(),vectorHugo.end());
    std::vector<char>::iterator pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\n');
    while(pos != m_requestBuffer.end())
    {
        std::string line =  std::string(m_requestBuffer.begin(), pos);
        if (m_firstLine == 1)
            this->fillReqLine(line);
        else if (line.size() == 0)
            m_bodyStart = 1;
        else if (m_bodyStart == 1)
            m_body.insert(m_body.end(), m_requestBuffer.begin(), pos + 1);
        else
            this->addHeader(line);
        m_requestBuffer.erase(m_requestBuffer.begin(), pos + 1);
        pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\n');
    }
    if (m_bodyStart == 1)
    {
        m_body.insert(m_body.end(), m_requestBuffer.begin(), m_requestBuffer.end());
        m_requestBuffer.clear();
    }
    else if (m_headerStart == 1)
    {
        this->addHeader(std::string(m_requestBuffer.begin(), m_requestBuffer.end()));
        m_requestBuffer.clear();
    }
}

// std::string        Request::isWellFormed(void)
// {
//     if ((m_method != "GET" && m_method != "POST" && m_method != "DELETE") || m_version != "HTTP/1.1")
//         return("400 Bad Request\n");
//     if (m_headers.find("Transfer-Encoding") != std::string::npos && m_headers.find("chunked") == std::string::npos)
//         return("501 Not Implemented\n");
//     if (m_headers.find("Transfer-Encoding") == std::string::npos && m_headers.find("Content-Length") == std::string::npos && m_method == "POST")
//         return("400 Bad Request\n");
//     if (m_uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
//         return("400 Bad Request\n");
//     if (m_uri.length() > 2048)
//         return("414 Request-URI Too Long\n");
//     else
//         return("200 OK\n");
// }

std::ostream& operator<<(std::ostream& out, Request request)
{
    out << "=============Request==============" << std::endl;
    out << "request sd: " << request.getSd() << std::endl;
    out << "request method: " << request.getMethod()<< std::endl;
    out << "request uri: " << request.getUri()<< std::endl;
    out << "request version: " << request.getVersion()<< std::endl;
    out << "request headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator i = request.getHeaders().begin(); i != request.getHeaders().end(); ++i)
	    out << "  " << i->first << ":" << i->second << std::endl;
    out << "request body: " << std::string(request.getBody().begin(), request.getBody().end())<< std::endl;
	return out;
}

int                                 Request::getSd(void) const{return(m_sd);}
std::string                         Request::getUri(void) const{return(m_uri);}
std::string                         Request::getVersion(void) const{return(m_version);}
std::vector<char>                   Request::getRequestBuffer(void) const{return(m_requestBuffer);}
std::string                         Request::getMethod(void)const{return(m_method);}
std::map<std::string, std::string>  &Request::getHeaders(void){return(m_headers);}
std::vector<char>                   &Request::getBody(void){return(m_body);}
int                                 Request::getFirstLine(void) const{return(m_firstLine);}
int                                 Request::getHeaderStart(void) const{return(m_headerStart);}
int                                 Request::getBodyStart(void) const{return(m_bodyStart);}
