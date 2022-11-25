#pragma once
#include "../webserv.hpp"

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
		size_t								m_bodyLength;
		bool								m_firstLine;
		bool								m_bodyStart;
		std::string							m_status;
		int									m_chunckLen;
		std::string							m_queryString;
		std::string							m_filePath;
		int									m_bodyfd;

	public:
		// Class methods:
		void								parse(const std::vector<Server> &servers, const char *buf, int bufSize);
		bool								fillReqLine(std::string line);
		void								addHeader(std::string line);
		void								fillBody(void);
		bool								isWellFormed(void);
		void								checkErrors(const std::vector<Server> &servers);
		bool								methodAllowed(void);
		void								matchLocation(void);
		bool								matchServer(const std::vector<Server> &servers);
		void    							fillChunked(void);
		void    							fillContentLength(size_t contentLength);
		// Class attributes getters :
		std::FILE							*getBody(void) const;
		int									getSd(void) const;
		std::map<std::string, std::string>	getHeaders(void) const;
		std::string							getMethod(void) const;
		std::string							getUri(void) const;
		std::string							getQueryString(void) const;
		std::string							getVersion(void) const;
		std::vector<char>					getRequestBuffer(void) const;
		std::string							getError(void) const;
		Server								getServer(void) const;
		Location							getLocation(void) const;
		std::string							getHeader(std::string key)const;
		std::string							getStatus(void)const;
		std::string                         getFilePath(void) const;
		int									getBodyfd(void) const;
		// operator overloads :
		Request&							operator= (const Request &req);
		// Class Constructors/destructor :
											Request(void);
											Request(const Request &cp);
											Request(int sd);
											~Request(void);
};

std::ostream& operator<<(std::ostream& os, Request request);
