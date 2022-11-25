/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:33 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/25 09:17:25 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"
// #include <iostream>
# define SENDING_DONE 0
# define SENDING_HEADERS 1
# define SENDING_BODY 2
# define RES_BUFFER_SIZE 10000
// environ
extern char **environ;

class Response
{
	private:
		std::string							m_statusCode;
		std::string							m_headers;
		std::map<std::string, std::string>	m_resHeaders;
		std::string							m_filePath;
		FILE								*m_file;
		std::string							m_buffer;
		char								m_smolBuffer[RES_BUFFER_SIZE];
		Request								m_request;
		bool								m_done;
		int									m_cursor;
		int									m_bodySize;
		long								m_lastSent;
		int									m_sd;
		std::string							m_cgiPath;
		std::string							m_absolutePath;
		Location							m_location;
		Server								m_server;
		

	public:
		std::string							getReqFilename(void);
		int									getReqBodyFd(void);
		bool								getKeepAlive();
		FILE*								getFile(void);
		bool								setFilePath();
		const Request 						&getRequest();
		std::string							getExtention();
		std::string							getCodeString();
		std::string							generateAutoIndex();
		bool								handleGetFile();
		bool								handleGet();
		bool								handlePost();
		bool								handleDelete();
		bool								handleCgi();
		bool								handleCgiResponse(FILE *tmp);
		void								setErrorPage();
		std::string							getHeader(std::string key) const;
		int									getSd();
		std::string							peek(bool &done);
		void								setLastSent(long sent);
		void								setCgiEnv();
		void								buildHeaders();
											Response(const Request &request);
											Response(const Response &src);
											Response();
											~Response();
		Response							&operator=(const Response &rhs);
		
		friend std::ostream& operator<<(std::ostream& out, const Response& response);
};
