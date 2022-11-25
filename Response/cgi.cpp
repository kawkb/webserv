/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:39 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/25 12:38:10 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	Response::setCgiEnv()
{
	setenv("PATH_INFO", m_request.getUri().substr(0, m_request.getUri().find_last_of("/")).c_str(), true);
	setenv("REQUEST_URI", m_request.getUri().substr(0, m_request.getUri().find_last_of("/")).c_str(), true);
	setenv("REDIRECT_STATUS", "200", true);
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
	std::string content_type = m_request.getHeader("Content-Type");
	if (!content_type.empty())
		setenv("CONTENT_TYPE", content_type.c_str(), true);
	else
		setenv("CONTENT_TYPE", "text/html", true);
	std::string content_length = m_request.getHeader("Content-Length");
	if (!content_length.empty() && m_request.getMethod() == "POST")
		setenv("CONTENT_LENGTH", content_length.c_str(), true);
	else
		setenv("CONTENT_LENGTH", "0", true);
	const std::map<std::string, std::string> headers = m_request.getHeaders();
	std::map<std::string, std::string>::const_iterator i;
	for (i = headers.begin();i != headers.end();i++)
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
	FILE *tmpfi = tmpfile();
	bool timeout = true;
	pid_t pid;
	pid = fork();
	if (pid == -1)
	{
		m_statusCode = "500";
		return false;
	}
	int status = 0;
	if (pid == 0)
	{
		struct stat st;
		if (stat(m_cgiPath.c_str(), &st) == -1)
			exit(69);
		if (S_ISDIR(st.st_mode))
			exit(69);
        const char *argv[] = {m_cgiPath.c_str(), m_filePath.c_str(), NULL};
		std::cout << "argv[0] = " << argv[0] << std::endl;
		std::cout << "argv[1] = " << argv[1] << std::endl;
		if (dup2(fileno(tmpfi), STDOUT_FILENO) == -1)
			exit (1);
		if (m_request.getMethod() == "POST")
		{
			FILE *reqbody = m_request.getBody();
			if (reqbody != NULL)
				dup2(fileno(reqbody), STDIN_FILENO);
		}
		execve(argv[0], (char **)argv, environ);
	}
	else
	{
		time_t t = time(NULL);
		while (time(NULL) - t < 5)
		{
			pid_t val = waitpid(pid, &status, WNOHANG);
			usleep(10000);
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
		{
			kill(pid, SIGKILL);
		}
	}
	if (WIFEXITED(status))
	{
		if (timeout)
		{
			m_statusCode = "408";
			return false;
		}
		else if (WEXITSTATUS(status) == 69)
		{
			m_statusCode = "500";
			return false;
		}

		else if (WEXITSTATUS(status) != 0)
		{
			m_statusCode = "500";
			return false;
		}
	}
	rewind(tmpfi);
	return handleCgiResponse(tmpfi);
}

bool	Response::handleCgiResponse(FILE *tmp)
{
	std::string cgi_response;
	char buf[1024];
	int ret = 0;
	// use normal read
	// add select
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fileno(tmp), &readfds);
	select(fileno(tmp) + 1, &readfds, NULL, NULL, NULL);
	while ( FD_ISSET(fileno(tmp), &readfds)  && (ret = fread(buf,1 , 1024, tmp)) > 0)
	{
		cgi_response += std::string(buf, ret);
		// stop reading if the response contains \r\n\r\n
		if (cgi_response.find("\r\n\r\n") != std::string::npos)
			break;
	}
	
	// find the end of the header
	size_t header_end = cgi_response.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		m_buffer = cgi_response;
		m_statusCode = "200";
		rewind(tmp);
		m_file = tmp;
		FILE*	body = m_request.getBody();
		if (body)
			fclose(body);
		m_bodySize = m_buffer.size();
		buildHeaders();
		return true;
	}
	// rewind by what's lest after the header
	fseek(tmp, header_end + 4, SEEK_SET);
	// printFile(tmp);
	m_file = tmp;

	std::cout << "header_end: " << header_end << std::endl;
	std::string headers = cgi_response.substr(0, header_end);
	std::vector<std::string> headerlines = split(headers, "\r\n");
	for (std::vector<std::string>::iterator i = headerlines.begin(); i != headerlines.end(); ++i)
	{
		size_t pos = i->find(":");
		if (pos == std::string::npos)
			m_resHeaders[*i] = "";
		else
		{
			std::string key = i->substr(0, pos);
			std::string value = i->substr(pos + 2);
			m_resHeaders[key] = value;
		}
	}
	std::string statusHeader = getHeader("Status");
	if (!statusHeader.empty())
		m_statusCode = statusHeader.substr(0, 3);
	else
		m_statusCode = "200";
	//calculate content lenght
	fseek(tmp, 0, SEEK_END);
	m_bodySize = ftell(tmp) - header_end - 4;
	fseek(tmp, header_end + 4, SEEK_SET);
	m_resHeaders.erase("Status");
	// if (m_resHeaders.find("Content-type") == m_resHeaders.end())
	// 	m_resHeaders["Content-Type"] = "text/html";
	FILE*	body = m_request.getBody();
	if (body)
		fclose(body);
	buildHeaders();
	return true;
}
