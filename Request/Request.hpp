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
		std::FILE							*m_body;
		bool								m_firstLine;
		bool								m_headerStart;
		bool								m_bodyStart;
		std::string							m_status;
		std::map<std::string, std::string>	m_queryString;
	public:
		// Class methods:
		void                                parse(const std::vector<Server> &servers, const char *buf, int bufSize);
		bool                                fillReqLine(std::string line);
		void                                addHeader(std::string line);
		void								fillQueryString();
		void								fillBody(void);
		bool								isWellFormed(void);
		void    							checkErrors(const std::vector<Server> &servers);
		bool								methodAllowed(void);
		bool								matchLocation(void);
		bool								matchServer(const std::vector<Server> &servers);
		// Class attributes getters:
		std::vector<char>                   getBody(void) const;
		int                                 getSd(void) const;
		std::map<std::string, std::string>  getHeaders(void) const;
		std::string                         getMethod(void) const;
		std::string                         getUri(void) const;
		std::string                         getVersion(void) const;
		std::vector<char>                   getRequestBuffer(void) const;
		std::string                         getError(void) const;
		Server                              getServer(void) const;
		Location                            getLocation(void) const;
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
