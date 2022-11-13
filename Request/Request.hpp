#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../configFileParser/Server.hpp"
std::vector<std::string>    tokenize(std::string line);

class Request
{
	private:
		int									m_sd;
		std::vector<char>					m_requestBuffer;
		std::string							m_method;
		std::string							m_uri;
		std::string							m_version;
		std::map<std::string, std::string>	m_headers;
		std::vector<char>					m_body;
		int									m_firstLine;
		int									m_headerStart;
		int									m_bodyStart;

	public:
		void                                parse(const char *buf, int bufSize);
		std::string                         isWellFormed(void);
		void                                fillReqLine(std::string line);
		void                                addHeader(std::string line);

		std::vector<char>                   getBody(void) const;
		int                                 getSd(void) const;
		std::map<std::string, std::string>  getHeaders(void) const;
		std::string                         getMethod(void) const;
		std::string                         getUri(void) const;
		std::string                         getVersion(void) const;
		std::vector<char>                   getRequestBuffer(void) const;
		int                                 getFirstLine(void) const;
		int                                 getHeaderStart(void) const;
		int                                 getBodyStart(void) const;
	    std::string            getHeader(std::string key)const;
		Request&                            operator= (const Request &req);

											Request(void);
											Request(const Request &cp);
											Request(int sd);
											~Request(void);
};
std::ostream& operator<<(std::ostream& os, Request request);

