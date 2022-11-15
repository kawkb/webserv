#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"
#include "../utils/utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
# define BUFFER_SIZE 1024
class Response
{
private:
	std::string			m_statusCode;
	std::string			m_headers;
	std::FILE*			m_bodyFile;
	Request				m_request;
	bool				m_headersSent;
	bool				m_bodySent;
	int					m_bodyCursor;
	int					m_bodySize;
	int					m_headersCursor;

public:
	static std::string	getCodeString(std::string code);
	static std::string	generateAutoIndex(std::string path);
	bool				handleGet(const Request &req);
	bool				handlePost(const Request &req);
	bool				handleDelete(const Request &req);
	bool				continueRes();
	std::string			getRequestHeader(std::string key)const;
	std::string			getRequestMethod(void) const;
	FILE				*getBodyFile(void) const;
	std::string			getCgiPath(void) const;
	bool				isDone() const;
	void				setErrorPage();
	int					getSd();
	void				increaseHeaderCursor(int cursor);
	std::string			peekHeaders();
	void				peekBody(char *buf, int *chunksize);
	std::string 		serveCgi(Request Request);
						Response(const Request &request);
						Response();
						~Response();
};
