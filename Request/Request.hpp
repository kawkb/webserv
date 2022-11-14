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
		Server								m_server;
		Location							m_location;
		std::map<std::string, std::string>	m_headers;
		std::vector<char>					m_body;
		bool								m_firstLine;
		bool								m_headerStart;
		bool								m_isDone;
		std::string							m_status;
	public:
		// Class methods:
		void                                parse(const char *buf, int bufSize);
		void                                fillReqLine(std::string line);
		void                                addHeader(std::string line);
		void								fillBody();
		void								isWellFormed();
		// Class attributes getters:
		std::vector<char>                   getBody(void) const;
		int                                 getSd(void) const;
		std::map<std::string, std::string>  getHeaders(void) const;
		std::string                         getMethod(void) const;
		std::string                         getUri(void) const;
		std::string                         getVersion(void) const;
		std::vector<char>                   getRequestBuffer(void) const;
		bool                                getFirstLine(void) const;
		bool                                getHeaderStart(void) const;
		bool                                getIsDone(void) const;
		std::string                         getError(void) const;
	    std::string            				getHeader(std::string key)const;
		Request&                            operator= (const Request &req);
		// Class Constructors/destructor			
											Request(void);
											Request(const Request &cp);
											Request(int sd);
											~Request(void);
};
std::ostream& operator<<(std::ostream& os, Request request);

//read more about the request on rfc 
// write the fillbody fnction
// test well your request;
// go to dinner
// socket all nighter : add the fcntl no blocking flag + timeout ;
// but first lets drink coffe with my baby
