/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:39 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/17 05:11:21 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	Response::setCgiEnv()
{
	std::string content_type = m_request.getHeader("Content-Type");
	if (!(content_type.empty()))
		setenv("CONTENT_TYPE", content_type.c_str(), 1);
	else
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	std::string content_length = m_request.getHeader("Content-Length");
	if (!(content_length.empty()))
		setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", m_request.getQueryString().c_str(), 1);
	setenv("REQUEST_METHOD", stringToUpper(m_request.getMethod()).c_str(), 1);
	setenv("SCRIPT_FILENAME", m_request.getServer().getCgiPath().c_str(), 1);
	setenv("SERVER_SOFTWARE", "Webserv", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

bool	Response::handleCgi()
{
	setCgiEnv();
	// create temporary file for cgi response using tempfile
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
	{
		return NULL;
	}
	// dup standard output to temporary file
	int stdout_copy = dup(1);
	dup2(fileno(tempfile), 1);
	// formulate environement variables
	// if get request, add query string to environement variables
	// if post request, add content type and content length to environement variables
	// if post request, dup standard input to request body file descriptor
	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (request.getMethod() == "POST")
		{
			int stdin_copy = dup(0);
			// write request body to temporary file
			FILE *body = tmpfile();
			if (body == NULL)
			{
				std::cout << "Error creating temporary file" << std::endl;
				return ;
			}
			std::string body_str = request.keys["body"];
			fwrite(body_str.c_str(), 1, body_str.length(), body);
			rewind(body);
			// dup standard input to temporary file
			dup2(fileno(body), 0);
			// execute cgi script
		}
		if (keys["extension"] == ".php")
		{
			char *args[3];
			args[0] = (char *)keys["cgi_path"].c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
		else if (keys["extension"] == ".py")
		{
			char *args[3];
			std::string python = "/usr/bin/python";
			args[0] = (char *)python.c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
 // environ is a variable declared in unistd.h, and it keeps track of the environment variables during this running process.
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
	// restore standard output
	dup2(stdout_copy, 1);
	close(stdout_copy);
	rewind(tempfile);
	return (tempfile);
}

