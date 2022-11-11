/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/10 14:56:45 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void        Response::fill(Request request)
{
}

Response::~Response(){}
Response::Response(){}

int       Response::handleGet()
{
    
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
    m_server = request.matchServer(servers);
    m_statusCode = request.isWellFormed();
    if (m_statusCode != "200 OK\n")
        setErrorPage();
    else if (request.getMethod() == "GET")
        handleGet();
    else if (request.getMethod() == "POST")
        handlePost();
    else if (request.getMethod() == "DELETE")
        handleDelete();
}