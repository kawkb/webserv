/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:12:34 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/26 06:08:06 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi(){}

cgi::cgi(std::string path, std::string type, Request req, std::string file):  _path(path), _type(type), _req(req), _file(file){}

cgi::~cgi(){}



// Fixed some issues here
// Used getKey; The iterator thing won't work as keys is private
// For the query part, commented out until we add on the request parsing.
void cgi::env()
{
	std::string reqtype = _req.getKey("reqtype");

	// There will always be a reqtype; so no need to check here. But a check might be done getKey level either throw an exception ot check if empty()
	if (reqtype == "GET")
	{
		std::string query = _req.getKey("query");	
		if (query.size() != 0)
		{
			std::string c_size = std::to_string(query.length());
			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}		
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (reqtype == "POST")
	{
		std::string content_type = _req.getKey("Content-Type");
		if (!(content_type.empty()))
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::string content_length = _req.getKey("Content-Length");
		if (!(content_length.empty()))
			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	}
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", _req.getKey("query").c_str(), 1);
	setenv("REQUEST_METHOD", reqtype.c_str(), 1);
	setenv("SCRIPT_FILENAME", _file.c_str(), 1); // Parse file on request level
	//setenv("SERVER_SOFTWARE", /*same as the server description*/, 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

/*void cgi::env()
{
	std::map<std::string, std::string>::iterator it = _req.find("_reqtype");
	if (it != _req.end())
	{
		if (it->second == "GET")
		{
			int s = _req.length();
			std::string c_size = toString(s);
			if (_req._query.size() != 0)
				setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (it->second == "POST")
	{
		std::map<std::string, std::string>::iterator ite = _req.find("content-type");
		if (ite != _req.end())
			setenv("CONTENT_TYPE", (ite->second)->c.str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::map<std::string, std::string>::iterator iter = _req.find("content-length");
		if (iter != _req.end())
			setenv("CONTENT_LENGTH", (iter->second).c_str(), 1);
	}
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", _req._query.c_str(), 1);
	setenv("_reqUEST_METHOD", (it->first).c_str(), 1);
	setenv("SCRIPT_FILENAME", _file.c_str(), 1);
	//setenv("SERVER_SOFTWARE", same as the server description, 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

*/