/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/11 19:25:01 by kdrissi-         ###   ########.fr       */
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

int     Request::getSd(void) const{return(m_sd);}
std::string Request::getMethod(void)const{return(m_method);}
std::string Request::getHeaders(void)const{return(m_headers);}

void    Request::parse(const char *buf, int bufSize)
{
    std::vector<char> vectorHugo;
    vectorHugo.assign(buf,buf+bufSize);
    m_requestBuffer.insert(m_requestBuffer.begin() + m_requestBuffer.size(), vectorHugo.begin(),vectorHugo.end());
    for(std::vector<char>::iterator pos = find(m_requestBuffer.begin(), m_requestBuffer.end(), '\n'); pos != m_requestBuffer.end();// delete till c)
    {
        std::string line =  std::string(m_requestBuffer.begin(), pos);
        if (m_firstLine == 1)
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
        else if (line.size() == 1)
            m_bodyStart = 1;
        else if (m_bodyStart == 1)
            m_body += line; 
        else
        {
            m_headerStart = 1;
            size_t found = line.find(':');
            while(found != std::string::npos)
            {
                
            }
            m_headersBuffer += line;
            
        }
    }
    if (m_bodyStart == 1)
    {
        m_body += m_requestBuffer;
        m_requestBuffer = "";
    }
    else if (m_headerStart == 1)
    {
        
        m_headers += m_requestBuffer; 
        m_requestBuffer = "";
    }
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
    else
        return("200 OK\n");
}

std::ostream& operator<<(std::ostream& out, Request request)
{
    out << "=============Request==============" << std::endl;
    std::string method = request.getMethod();
	out << "request port: " << request.getPort() << std::endl;
    out << "request name: " << request.getName() << std::endl;
    out << "request method: ";
	for (long unsigned int i = 0; i < method.size(); i++)
	    out << method[i] << " ";
    out << std::endl;
	return out;
}
