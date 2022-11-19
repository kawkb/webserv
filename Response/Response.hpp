/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:33 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/18 19:28:43 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"
#include "../utils/utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

# define SENDING_DONE 0
# define SENDING_HEADERS 1
# define SENDING_BODY 2

// environ
extern char **environ;

class Response
{
	private:
		std::string							m_statusCode;
		std::string							m_headers;
		std::map<std::string, std::string>	m_headersMap;
		std::string							m_filePath;
		std::FILE*							m_bodyFile;
		Request								m_request;
		bool								m_headersSent;
		bool								m_bodySent;
		int									m_bodyCursor;
		int									m_bodySize;
		int									m_headersCursor;

	public:
		std::string							getExtention();
		std::string							getCodeString();
		FILE								*generateAutoIndex();
		bool								handleGetFile(off_t filesize);
		bool								handleGet();
		bool								handlePost();
		bool								handleDelete();
		bool								handleCgi();
		void								setErrorPage();
		std::string							getHeader(std::string key);
		int									getSd();
		void								moveHeaderCursor(int cursor);
		void								moveBodyCursor(int cursor);
		bool								peekHeaders(char *buf, long *sendSize);
		bool								peekBody(char *buf, long *sendSize);
		int									peek(char *buf, long *sendSize);
		void								setCgiEnv();
		void								buildHeaders();
											Response(const Request &request);
											Response();
											~Response();
};
