#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"

#include <dirent.h>
#include <sys/stat.h>

// #include <sys/stat.h>


class Response
{
private:
	int 		m_sd;
	std::string	m_statusCode;
	std::string m_body;
	Server 		m_server;
	Location	m_location;
	std::string m_serverName;
	std::string m_response;

public:
	static std::string	getCodeString(std::string code);
	static std::string	generateAutoIndex(std::string path);
	bool	handleGet(const Request &req);
	bool	handlePost(const Request &req);
	bool	handleDelete(const Request &req);
	void	setErrorPage();
	int		methodAllowed(const Request &request);
	bool	matchLocation(const Request &request);
	bool	matchServer(const std::vector<Server> &servers, const Request &request);
	bool	isWellFormed(const Request &request);
			Response(const Request &request, const std::vector<Server> &servers);
			Response();
			~Response();
};
