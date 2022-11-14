#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"

#include <dirent.h>
#include <sys/stat.h>

class Response
{
private:
	int 		m_sd;
	std::string	m_statusCode;
	std::string m_body;
	std::string m_serverName;
	char*		m_response;
	Request		m_request;
	bool		m_done;
	std::string m_readcursor;

public:
	static std::string	getCodeString(std::string code);
	static std::string	generateAutoIndex(std::string path);
	bool	handleGet(const Request &req);
	bool	handlePost(const Request &req);
	bool	handleDelete(const Request &req);
	bool	continueRes();
	bool	isDone() const;
	bool	flushRes(int readsize);
	void	setErrorPage();
			Response(const Request &request);
			Response();
			~Response();
};
