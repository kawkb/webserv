/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:45:33 by moerradi          #+#    #+#             */
/*   Updated: 2022/11/24 08:39:31 by kdrissi-         ###   ########.fr       */
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
		std::map<std::string, std::string>	m_headersMap;
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

	public:
		const Request 						&getRequest();
		std::string							getExtention();
		std::string							getCodeString();
		std::string							generateAutoIndex();
		bool								handleGetFile();
		bool								handleGet();
		bool								handlePost();
		bool								handleDelete();
		bool								handleCgi();
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
