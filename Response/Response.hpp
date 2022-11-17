/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:33 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/16 18:23:36 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"
#include "../utils/utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

# define SENDING_DONE 0
# define SENDING_HEADERS 1
# define SENDING_BODY 2

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
		std::string							getCodeString();
		FILE								*generateAutoIndex();
		bool								handleGet();
		bool								handlePost();
		bool								handleDelete();
		std::string							getRequestHeader(std::string key)const;
		std::string							getRequestMethod(void) const;
		std::string							getFilePath(void);
		FILE								*getBodyFile(void) const;
		bool								isDone() const;
		void								setErrorPage();
		int									getSd();
		void								moveHeaderCursor(int cursor);
		void								moveBodyCursor(int cursor);
		bool								peekHeaders(char *buf, long *sendSize);
		bool								peekBody(char *buf, long *sendSize);
		int									peek(char *buf, long *sendSize);
		bool								handleCgi();
		void								setCgiEnv();
		void								buildHeaders();
											Response(const Request &request);
											Response();
											~Response();
};
