/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 01:05:43 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/10/29 20:15:54 by kdrissi-         ###   ########.fr       */
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
    m_request = "";
    m_requestLine = "";
    m_bodyStart = 0;
    m_firstLine = 1;
}

int     Request::getSd(void) const
{
    return(m_sd);
}

// int     fill_request_line(std::string line)
// {
//     std::vector<std::string> tokens;
//     tokens = tokenize(line);
//     for(std::vector<std::string>::iterator it = tokens.begin(); i != tokens.end(); ++i)
//     {
//         if(*i == "GET" || *i == "POST" || *i == "DELETE")
//             m_method = *i;
//         else if(*i == "HTTP/1.1")
//             m_version = *i;
//         else
//             m_uri == *i;
//     }
    
// }

// int     fill_headers(std::string)
// {
    
// }

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
            m_firstLine = 0;
            std::vector<std::string> tokens;
            tokens = tokenize(line);
            for(std::vector<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
            {
                if(*i == "GET" || *i == "POST" || *i == "DELETE")
                    m_method = *i;
                else if(*i == "HTTP/1.1")
                   m_version = *i;
                else
                    m_uri == *i;
        }
        }
        if (line.empty())
            m_bodyStart = 1;
        if (m_bodyStart == 1)
            m_body += line;
        else
            m_headears += line; 
            // fill_headers(line);
        m_request.erase(0, pos + 1);
    }
}
// GET / HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f
// Host: localhost:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive