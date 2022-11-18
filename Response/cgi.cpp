/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:39 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/18 12:43:00 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	Response::setCgiEnv()
{
	std::string content_type = m_request.getHeader("Content-Type");
	if (!(content_type.empty()))
		setenv("CONTENT_TYPE", content_type.c_str(), 1);
	else
		setenv("CONTENT_TYPE", "text/html", 1);
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
	// set cookie env
	std::string cookie = m_request.getHeader("Cookie");
	if (!(cookie.empty()))
		setenv("HTTP_COOKIE", cookie.c_str(), 1);
}

bool	Response::handleCgi()
{
	setCgiEnv();
	bool timeout = true;
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
		return false;

	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (dup2(fileno(tempfile), 1) == -1)
			exit (1);
		if (m_request.getMethod() == "POST")
		{
			if (m_bodyFile != NULL)
				dup2(fileno(m_bodyFile), 0);
		}
		char *args[3];
		args[0] = (char *)m_request.getServer().getCgiPath().c_str();
		args[1] = (char *)m_filePath.c_str();
		args[2] = NULL;
		execve(args[0], args, environ);
	}
	else
	{
		time_t t = time(NULL);
		while (time(NULL) - t < 5)
		{
			pid_t val = waitpid(pid, &status, WNOHANG);
			if (val == -1)
				return false;
			if (val != 0)
			{
				timeout = false;
				break;
			}
		}
		if (timeout)
			kill(pid, SIGTERM);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 143)
		{
			m_statusCode = "408";
			return false;
		}
		else if (WEXITSTATUS(status) != 0)
		{
			m_statusCode = "500";
			return false;
		}
	}
	// read response headers from tempfile
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, tempfile)) != -1)
	{
		if (strcmp(line, "\r\n") == 0)
			break;
	}
	return true;
}

