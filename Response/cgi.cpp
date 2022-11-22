/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:39 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/22 05:06:11 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	Response::setCgiEnv()
{
	setenv("REQUEST_URI", m_request.getUri().substr(0, m_request.getUri().find_last_of("/")).c_str(), true);
	std::string document_uri = m_request.getUri() + "?" + m_request.getQueryString();
    setenv("DOCUMENT_URI", document_uri.c_str(), true);
    setenv("SCRIPT_NAME",  m_request.getUri().c_str(), true);
    setenv("SCRIPT_FILENAME", m_filePath.c_str(), true);
    setenv("PATH_TRANSLATED", m_filePath.c_str(), true);
    setenv("QUERY_STRING", m_request.getQueryString().c_str(), true);
    setenv("SERVER_NAME","localhost", true);
    setenv("SERVER_PORT", toString(m_request.getServer().getPort()).c_str(), true);
    setenv("REQUEST_METHOD", m_request.getMethod().c_str(), true);
    setenv("DOCUMENT_ROOT", m_request.getLocation().getRoot().c_str(), true);
    setenv("GETAWAY_INTERFACE","CGI/1.1", true);
    setenv("SERVER_PROTOCOL","HTTP/1.1", true);
    setenv("FCGI_ROLE","RESPONDER", true);
    setenv("REQUEST_SCHEME","http", true);
    setenv("SERVER_SOFTWARE","webserv/1.1", true);
    setenv("REMOTE_ADDR","0.0.0.0", true);
    setenv("REMOTE_PORT","0", true);
	std::string content_type = m_request.getHeader("Content-Type");
	if (!content_type.empty())
		setenv("CONTENT_TYPE", content_type.c_str(), true);
	else
		setenv("CONTENT_TYPE", "text/html", true);
	std::string content_length = m_request.getHeader("Content-Length");
	if (!content_length.empty())
		setenv("CONTENT_LENGTH", content_length.c_str(), true);
	else
		setenv("CONTENT_LENGTH", "0", true);
	std::map<std::string, std::string>::iterator i;
	for (i= m_request.getHeaders().begin();i != m_request.getHeaders().end();i++)
	{
		std::string key = "HTTP_" + i->first;
		std::replace(key.begin(), key.end(), '-', '_');
		key = stringToUpper(key);
		setenv(key.c_str(), i->second.c_str(), true);
	}
}

bool	Response::handleCgi()
{
	setCgiEnv();
	bool timeout = true;
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
	{
		m_statusCode = "500";
		return false;
	}

	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (dup2(fileno(tempfile), 1) == -1)
			exit (1);
		if (m_request.getMethod() == "POST")
		{
			FILE *reqbody = m_request.getBody();
			if (reqbody != NULL)
				dup2(fileno(reqbody), 0);
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
			usleep(1000);
			if (val == -1)
			{
				m_statusCode = "500";
				return false;
			}
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
		std::string header = line;
		std::string key = header.substr(0, header.find(":"));
		std::string value = header.substr(header.find(":") + 2);
		if (value.size() > 2)
			value = value.substr(0, value.size() - 2);
		m_headersMap[key] = value;
		free(line);
		line = NULL;
	}
	if (errno == EINVAL || errno == ENOMEM)
	{
		m_statusCode = "500";
		return false;
	}
	if (line)
		free(line);
	FILE *tempfile2 = tmpfile();
	if (tempfile2 == NULL)
	{
		m_statusCode = "500";
		return false;
	}
	char buf[BUFFER_SIZE];
	while ((read = fread(buf, 1, BUFFER_SIZE, tempfile)) > 0)
		m_buffer += std::string(buf);
	m_done = true;
	std::string statusHeader = getHeader("Status");
	if (!statusHeader.empty())
		m_statusCode = statusHeader.substr(0, 3);
	else
		m_statusCode = "200";
	m_headersMap.erase("Status");
	if (m_headersMap.find("Content-Type") == m_headersMap.end())
		m_headersMap["Content-Type"] = "text/html";
	m_headersMap["Content-Length"] = toString(m_bodySize);
	return true;
}

