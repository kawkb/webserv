/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:39 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/23 00:20:12 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	Response::setCgiEnv()
{
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
		// needs further investigation segfaults when uncommented
	// std::map<std::string, std::string>::iterator i;
	// for (i= m_request.getHeaders().begin();i != m_request.getHeaders().end();i++)
	// {
	// 	std::string key = "HTTP_" + i->first;
	// 	std::replace(key.begin(), key.end(), '-', '_');
	// 	key = stringToUpper(key);
	// 	setenv(key.c_str(), i->second.c_str(), true);
	// }
}

void	printFile(FILE *fp)
{
	char buf[1024];
	while (fgets(buf, 1024, fp))
	{
		std::cout << buf;
	}
}

bool	Response::handleCgi()
{
	setCgiEnv();
	std::string cgi_path = m_request.getServer().getCgiPath();
	int pipefds[2];
	if (pipe(pipefds) == -1)
	{
		m_statusCode = "500";
		return false;
	}
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
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			exit (1);
		if (m_request.getMethod() == "POST")
		{
			FILE *reqbody = m_request.getBody();
			if (reqbody != NULL)
				dup2(fileno(reqbody), 0);
		}
        const char *argv[] = {cgi_path.c_str(), m_filePath.c_str(), NULL};
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
			close(pipefds[1]);
			kill(pid, SIGKILL);
		}
	}
	if (WIFEXITED(status))
	{
		std::cout << "exit status: " << WEXITSTATUS(status) << std::endl;
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
		close(pipefds[1]);
	}
	std::string cgi_response;
	char buf[1024];
	// use normal read
	while (read(pipefds[0], buf, 1024) > 0)
	{
		cgi_response += buf;
	}
	close(pipefds[0]);
	std::cout << "cgi response: " << cgi_response << std::endl;
	// read response headers from tempfile
	// FILE *fp = fdopen(pipefds[1], "r");
	// char *line = NULL;
	// size_t len = 0;
	// ssize_t read;
	// while ((read = getline(&line, &len, fp)) != -1)
	// {
	// 	if (strcmp(line, "\r\n") == 0)
	// 		break;
	// 	std::string header = line;
	// 	std::string key = header.substr(0, header.find(":"));
	// 	std::string value = header.substr(header.find(":") + 2);
	// 	if (value.size() > 2)
	// 		value = value.substr(0, value.size() - 2);
	// 	m_headersMap[key] = value;
	// 	free(line);
	// 	line = NULL;
	// }
	// if (errno == EINVAL || errno == ENOMEM)
	// {
	// 	m_statusCode = "500";
	// 	return false;
	// }
	// if (line)
	// 	free(line);
	// FILE *tempfile2 = tmpfile();
	// if (tempfile2 == NULL)
	// {
	// 	m_statusCode = "500";
	// 	return false;
	// }
	// char buf[BUFFER_SIZE];
	// while ((read = fread(buf, 1, BUFFER_SIZE, fp)) > 0)
	// 	m_buffer += std::string(buf);
	// m_done = true;
	// std::string statusHeader = getHeader("Status");
	// if (!statusHeader.empty())
	// 	m_statusCode = statusHeader.substr(0, 3);
	// else
	// 	m_statusCode = "200";
	// m_headersMap.erase("Status");
	// if (m_headersMap.find("Content-Type") == m_headersMap.end())
	// 	m_headersMap["Content-Type"] = "text/html";
	buildHeaders();
	return true;
}

