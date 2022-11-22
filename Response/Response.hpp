/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:33 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/22 02:17:40 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"
# define SENDING_DONE 0
# define SENDING_HEADERS 1
# define SENDING_BODY 2
# define RES_BUFFER_SIZE 4096
// environ
extern char **environ;

class Response
{
	private:
		std::string							m_statusCode;
		std::string							m_headers;
		std::map<std::string, std::string>	m_headersMap;
		std::string							m_filePath;
		std::fstream						m_file;
		std::string							m_buffer;
		char								m_smolBuffer[RES_BUFFER_SIZE];
		Request								m_request;
		bool								m_done;
		int									m_cursor;
		int									m_bodySize;
		long								m_lastSent;

	public:
		std::string							getExtention();
		std::string							getCodeString();
		std::string							generateAutoIndex();
		bool								handleGetFile(off_t filesize);
		bool								handleGet();
		bool								handlePost();
		bool								handleDelete();
		bool								handleCgi();
		void								setErrorPage();
		std::string							getHeader(std::string key);
		int									getSd();
		std::string							peek(bool &done);
		void								setLastSent(long sent);
		void								setCgiEnv();
		void								buildHeaders();
											Response(const Request &request);
											Response();
											~Response();
};
