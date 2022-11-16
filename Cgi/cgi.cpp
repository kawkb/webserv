/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ziyad <ziyad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 17:02:16 by ziyad             #+#    #+#             */
/*   Updated: 2022/11/15 19:20:14 by ziyad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern char **environ;

Response Cgi::getResponse()
{
	return (response);
}

void	Cgi::setenvCgi()
{
	if (response.getRequestMethod() == "GET")
	{
		std::string query = response.getRequestQuery();	
		if (query.size() != 0)
		{
			std::string c_size = std::to_string(response.getRequestQuery().length());
			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}		
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (response.getRequestMethod() == "POST")
	{
		std::string content_type = response.getRequestHeader("Content-Type");
		if (!(content_type.empty()))
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::string content_length = response.getRequestHeader("Content-Length");
		if (!(content_length.empty()))
			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	}
	
	if (response.getRequestHeader("cookie").size() != 0)
		setenv("HTTP_COOKIE", response.getRequestHeader("cookie").c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", response.getRequestQuery().c_str(), 1);
	setenv("REQUEST_METHOD", response.getRequestMethod().c_str(), 1);
	setenv("SCRIPT_FILENAME", response.getCgiPath().c_str(), 1); // Parse file on request level
	setenv("SERVER_SOFTWARE", "Test", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

FILE    *Cgi::serveCgi()
{
	setenvCgi();
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
	{
		std::cout << "Error creating temporary file" << std::endl;
		return ;
	}
	int stdout_copy = dup(1);
	dup2(fileno(tempfile), 1);
	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (response.getRequestMethod() == "POST")
		{
			int stdin_copy = dup(0);
			FILE *body = response.getBodyFile()	;
			if (body == NULL)
			{
				std::cout << "Error creating temporary file" << std::endl;
				return ;
			}
			dup2(fileno(body), 0);
		}
		if (response.getCgiPath().empty())
			return ;
		char *args[3];
		args[0] = (char *)response.getCgiPath().c_str();
		args[1] = (char *)response.getFilePath().c_str();
		args[2] = NULL;
		execve(args[0], args, environ);
	}
	else
	{
		time_t t = time(NULL);
		while (time(NULL) - t < 5)
		{
			if (waitpid(pid, &status, WNOHANG) != 0)
				break;
		}
	}
	dup2(stdout_copy, 1);
	close(stdout_copy);
	rewind(tempfile);
	return (tempfile);
}

Cgi::Cgi(Response &response): response(response)
{
}

Cgi::~Cgi()
{
}