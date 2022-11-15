#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"
#include "../utils/utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
class Response
{
private:
	std::string			m_statusCode;
	std::string			m_headers;
	std::string			m_filePath;
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
	bool				handleGet();
	bool				handlePost();
	bool				handleDelete();
	std::string			getRequestHeader(std::string key)const;
	std::string			getRequestMethod(void) const;
	std::string			getCgiPath(void) const;
	std::string			getFilePath(void);
	FILE				*getBodyFile(void) const;
	bool				isDone() const;
	void				setErrorPage();
	int					getSd();
	void				increaseHeaderCursor(int cursor);
	void				increaseBodyCursor(int cursor);
	bool				peekHeaders(char *buf, long *sendSize);
	bool				peekBody(char *buf, long *sendSize);
	bool				peek(char *buf, long *sendSize);
	std::string 		serveCgi(Request Request);
						Response(const Request &request);
						Response();
						~Response();
};
