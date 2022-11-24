/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 06:23:05 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string Response::getExtention()
{
	size_t pos = m_filePath.find_last_of(".");
	std::string extenstion = "";
	if (pos != std::string::npos)
		extenstion = m_filePath.substr(pos + 1);
	return extenstion;
}

std::string	Response::getHeader(std::string key) const
{
	std::map<std::string, std::string>::const_iterator it = m_headersMap.find(key);
	if (it != m_headersMap.end())
		return (it->second);
	return ("");
}

std::string	Response::getCodeString()
{
	if (m_statusCode == "200")
		return "OK";
	if (m_statusCode == "201")
		return "Created";
	if (m_statusCode == "202")
		return "Accepted";
	if (m_statusCode == "204")
		return "No Content";
	if (m_statusCode == "301")
		return "Moved Permanently";
	if (m_statusCode == "302")
		return "Found";
	if (m_statusCode == "304")
		return "Not Modified";
	if (m_statusCode == "400")
		return "Bad Request";
	if (m_statusCode == "401")
		return "Unauthorized";
	if (m_statusCode == "403")
		return "Forbidden";
	if (m_statusCode == "404")
		return "Not Found";
	if (m_statusCode == "405")
		return "Method Not Allowed";
	if (m_statusCode == "406")
		return "Not Acceptable";
	if (m_statusCode == "408")
		return "Request Timeout";
	if (m_statusCode == "409")
		return "Conflict";
	if (m_statusCode == "413")
		return "Payload Too Large";
	if (m_statusCode == "414")
		return "URI Too Long";
	if (m_statusCode == "415")
		return "Unsupported Media Type";
	if (m_statusCode == "500")
		return "Internal Server Error";
	if (m_statusCode == "501")
		return "Not Implemented";
	if (m_statusCode == "502")
		return "Bad Gateway";
	if (m_statusCode == "503")
		return "Service Unavailable";
	if (m_statusCode == "505")
		return "HTTP Version Not Supported";
	return "Unknown";
}

std::string	Response::generateAutoIndex()
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	std::string autoindexHtml;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(m_filePath.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				if (ent->d_type == DT_DIR)
					dirs.push_back(ent->d_name);
				else
					files.push_back(ent->d_name);
			}
		}
		closedir(dir);
	}
	else
	{
		return NULL;
	}
	std::sort(files.begin(), files.end());
	std::sort(dirs.begin(), dirs.end());
	autoindexHtml += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " + m_filePath + "</title>\n</head>\n<body>\n<h1>Index of " + m_filePath + "</h1>\n<hr>\n<table>\n<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>\n";
	autoindexHtml += "<tr><td><a href=\"../\">../</a></td><td></td><td></td></tr>\n";
	for (std::vector<std::string>::iterator i = dirs.begin(); i != dirs.end(); i++)
	{
		std::string folderpath = m_filePath + "/" + *i;
		struct stat attrib;
		if (stat(folderpath.c_str(), &attrib) < 0)
			return NULL;
		std::string lastModified = ctime(&attrib.st_mtime);
		autoindexHtml += "<tr><td><a href=\"" + *i + "/\">" + *i + "/</a></td><td>" + lastModified + "</td><td></td></tr>\n";
	}
	for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::string filePath = m_filePath + "/" + *i;
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) < 0)
			return NULL;
		std::string lastModified = ctime(&fileStat.st_mtime);
		autoindexHtml += "<tr><td><a href=\"" + *i + "\">" + *i + "</a></td><td>" + lastModified + "</td><td>" + toString(fileStat.st_size) + "</td></tr>\n";
	}
	autoindexHtml += "</table>\n<hr>\n</body>\n</html>";
	autoindexHtml += "<style>\nbody {\nfont-family: sans-serif;\n}\n\nh1 {\nfont-size: 1.5em;\n}\n\nhr {\nmargin: 1em 0;\n}\n\ntable {\nborder-collapse: collapse;\nwidth: 100%;\n}\n\ntd, th {\nborder: 1px solid #ccc;\npadding: 0.5em;\n}\n\nth {\nbackground: #eee;\n}\n\ntr:nth-child(even) {\nbackground: #f8f8f8;\n}\n</style>";
	m_bodySize = autoindexHtml.size();

	return autoindexHtml;
}

std::string	getContentType(std::string filename)
{
	std::string extension = filename.substr(filename.find_last_of(".") + 1);
	if (extension == "html" || extension == "htm")
		return "text/html";
	else if (extension == "css")
		return "text/css";
	else if (extension == "js")
		return "application/javascript";
	else if (extension == "jpg" || extension == "jpeg")
		return "image/jpeg";
	else if (extension == "png")
		return "image/png";
	else if (extension == "gif")
		return "image/gif";
	else if (extension == "swf")
		return "application/x-shockwave-flash";
	else if (extension == "ico")
		return "image/x-icon";
	else if (extension == "xml")
		return "text/xml";
	else if (extension == "pdf")
		return "application/pdf";
	else if (extension == "zip")
		return "application/zip";
	else if (extension == "gz")
		return "application/x-gzip";
	else if (extension == "tar")
		return "application/x-tar";
	else if (extension == "svg")
		return "image/svg+xml";
	else if (extension == "txt")
		return "text/plain";
	else if (extension == "mp3")
		return "audio/mpeg";
	else if (extension == "wav")
		return "audio/x-wav";
	else if (extension == "mpeg" || extension == "mpg" || extension == "mpe")
		return "video/mpeg";
	else if (extension == "qt" || extension == "mov")
		return "video/quicktime";
	else if (extension == "avi")
		return "video/x-msvideo";
	else if (extension == "doc")
		return "application/msword";
	else if (extension == "rtf")
		return "application/rtf";
	else if (extension == "xls")
		return "application/vnd.ms-excel";
	else if (extension == "ppt")
		return "application/vnd.ms-powerpoint";
	else if (extension == "wml")
		return "text/vnd.wap.wml";
	else if (extension == "wmlc")
		return "application/vnd.wap.wmlc";
	else if (extension == "wmls")
		return "text/vnd.wap.wmlscript";
	else if (extension == "wmlsc")
		return "application/vnd.wap.wmlscriptc";
	else
		return "application/octet-stream";
}

void		Response::buildHeaders()
{
	if (m_statusCode == "")
		m_statusCode = "200";
	std::string headers = "HTTP/1.1 " + m_statusCode + " " + getCodeString() + "\r\n";
	// disable cache
	headers += "Cache-Control: no-cache, no-store, must-revalidate\r\n";
	if (m_bodySize > 0)
		headers += "Content-Length: " + toString(m_bodySize) + "\r\n";
	std::map<std::string, std::string>::iterator i;
	for (i = m_headersMap.begin(); i != m_headersMap.end(); i++)
		headers += i->first + ": " + i->second + "\r\n";
	headers += "\r\n";
	m_buffer = headers;
}

int remove_directory(const std::string path)
{
	DIR *d = opendir(path.c_str());
	int r = -1;
	if (d)
	{
		struct dirent *p;
		r = 0;
		while (!r && (p = readdir(d)))
		{
			int r2 = -1;
			std::string filename(p->d_name);
			if (filename == "." || filename == "..")
				continue;
			const std::string filepath = path + "/" + filename;
			struct stat statbuf;
			if (!stat(filepath.c_str(), &statbuf))
			{
				if (S_ISDIR(statbuf.st_mode))
					r2 = remove_directory(filepath);
				else
					r2 = unlink(filepath.c_str());
			}
			r = r2;
		}
		closedir(d);
	}
	if (!r)
	  r = rmdir(path.c_str());
	return r;
}

bool Response::handleDelete()
{
	// location path should never end with a slash except if it is literally /
	// root path should always end with a slash
	Location location = m_request.getLocation();
	Server server = m_request.getServer();
	std::string path = location.getPath();
	std::string root = location.getRoot();
	std::string uri = m_request.getUri();
	// parse request path
	m_filePath = root + uri.substr(path.size());
	// resolve path
	std::string absolute = getAbsolutePath(m_filePath);
	if (!startsWith(absolute + "/", root))
	{
		m_statusCode = "403";
		return false;
	}
	struct stat fileStat;
	if (stat(m_filePath.c_str(), &fileStat) != 0)
	{
		if (errno == EACCES)
			m_statusCode = "403";
		else if (errno == ENOENT)
			m_statusCode = "404";
		else
			m_statusCode = "500";
		return false;
	}
	if (S_ISDIR(fileStat.st_mode))
	{
		if (m_filePath[m_filePath.size() - 1] != '/')
		{
			m_statusCode = "409";
			return false;
		}
		if (remove_directory(m_filePath) != 0)
		{
			if (errno == EACCES)
				m_statusCode = "403";
			else if (errno == ENOENT)
				m_statusCode = "404";
			else
				m_statusCode = "500";			
			return false;
		}
	}
	else if (S_ISREG(fileStat.st_mode))
	{
		std::string ext = getExtention();
		if (ext == server.getCgiExtention() && ext != "")
		
		if (unlink(m_filePath.c_str()) != 0)
		{
			if (errno == EACCES)
				m_statusCode = "403";
			else if (errno == ENOENT)
				m_statusCode = "404";
			else
				m_statusCode = "500";
			return false;
		}
	}
	else
	{
		m_statusCode = "403";
		return false;
	}
	m_statusCode = "204";
	return true;
}

bool		Response::handleGetFile(off_t filesize)
{
	Server server = m_request.getServer();
	std::string extension = m_filePath.substr(m_filePath.find_last_of(".") + 1);
	if (extension == server.getCgiExtention())
	{
		return handleCgi();
	}
	else
	{
		m_bodySize = filesize;
		m_headersMap["Content-Type"] = getContentType(m_filePath);
		m_file = fopen(m_filePath.c_str(), "r");
		if (!m_file)
		{
			m_statusCode = "500";
			return false;
		}
		buildHeaders();
		return true;
	}
}

bool		Response::handleGet()
{
	// location path should never end with a slash except if it is literally /
	// root path should always end with a slash
	Location location = m_request.getLocation();
	std::string path = location.getPath();
	std::string root = location.getRoot();
	std::string uri = m_request.getUri();
	m_filePath = root + uri.substr(path.size());
	// parse request path
	if (uri == path || uri == path + "/")
		m_filePath = root + location.getIndex();
	else
		m_filePath = root + uri.substr(path.size() + 1);
	// resolve path
	std::string absolute = getAbsolutePath(m_filePath);
	if (!startsWith(absolute + "/", root))
	{
		m_statusCode = "403";
		return false;
	}
	if (absolute != m_filePath)
	{
		const std::string host = m_request.getServer().getName() + ":" + toString(m_request.getServer().getPort());
		const std::string redirect = "http://" + host +  path + absolute.substr(root.size() - 1) + m_request.getQueryString();
		m_statusCode = "301";
		m_headersMap["Location"] = redirect;
		buildHeaders();
		return true;
	}

	struct stat fileStat;
	if (stat(m_filePath.c_str(), &fileStat) != 0)
	{
		if (errno == EACCES)
			m_statusCode = "403";
		else if (errno == ENOENT)
			m_statusCode = "404";
		else
			m_statusCode = "500";
		return false;
	}

	if (S_ISDIR(fileStat.st_mode))
	{
		if (m_filePath[m_filePath.size() - 1] != '/')
		{
			m_statusCode = "301";
			const std::string host = m_request.getServer().getName() + ":" + toString(m_request.getServer().getPort());
			const std::string redirect = "http://" + host +  uri + "/" + m_request.getQueryString();
			m_headersMap["Location"] = uri + "/";
			buildHeaders();
			return true;
		}
		else if (location.getAutoIndex())
		{
			m_headersMap["Content-Type"] = "text/html";
			m_statusCode = "200";
			buildHeaders();
			m_buffer += generateAutoIndex();
			return true;
		}
		else
		{
			m_statusCode = "403";
			return false;
		}
	}
	else if (S_ISREG(fileStat.st_mode))
		return handleGetFile(fileStat.st_size);
	else
	{
		m_statusCode = "403";
		return false;
	}
}

bool		Response::handlePost()
{
	// std::string uploadPath = m_request.getLocation().getUploadPath();
	// if (!uploadPath.empty())
	// {
	// 	std::string uri = m_request.getUri();
	// 	std::string path = m_request.getLocation().getPath();
	// 	std::string filePath = uploadPath + uri.substr(path.size());
	// 	std::string absolute = getAbsolutePath(filePath);

	// }
	return true;
}

void		Response::setErrorPage()
{
	std::string errorPagePath = m_request.getServer().getErrorPage(m_statusCode);
	std::string errorPage;
	if (errorPagePath.empty())
	{
		m_headersMap["Content-Type"] = "text/html";
		errorPage = "<html><head><title>" + m_statusCode + " " + getCodeString() + "</title></head><body>";
		errorPage += "<div class=\"base\">\n\t<div class=\"point\">></div>\n\t<h1><i>Http Error " + m_statusCode + ":</i> <br>";
		std::string errorString = getCodeString();
		for (size_t i = 0; i < errorString.size(); i++)
		{
			if (errorString[i] == ' ')
				errorString.replace(i, 1, "<br>");
		}
		errorPage += errorString + "</h1>\n</div></body></html>";
		errorPage += "<style>body {\n  background-color: #23307e;\n}\nbody .base {\n  background-color: #23307e;\n  width: 100%;\n  height: 100vh;\n  display: flex;\n  align-items: center;\n  justify-content: center;\n  flex-direction: column;\n  position: relative;\n}\nbody .base .point {\n  font-family: \"Ubuntu\", sans-serif;\n  font-size: 10vw;\n  position: absolute;\n  top: 2vh;\n  color: #f2e9df;\n  left: 10vw;\n}\nbody .base .point:after {\n  content: \"_\";\n  animation: sparkle 0.5s infinite;\n  position: absolute;\n}\nbody .base h1 {\n  color: #ff3d57;\n  font-family: \"Ubuntu\", sans-serif;\n  text-transform: uppercase;\n  font-size: 6vw;\n  position: absolute;\n  top: 20vh;\n  left: 10vw;\n  -webkit-tap-highlight-color: rgba(255, 255, 255, 0);\n}\n@media only screen and (max-width: 992px) {\n  body .base h1 {\n    font-size: 10vw;\n  }\n}\nbody .base h1.glitch {\n  animation: clap 0.1s 5 forwards;\n}\n\n@keyframes sparkle {\n  0%, 100% {\n    opacity: 0;\n  }\n  50% {\n    opacity: 1;\n  }\n}\n@keyframes clap {\n  0%, 100% {\n    opacity: 1;\n  }\n  30% {\n    left: 11vw;\n    color: #f2e9df;\n  }\n  70% {\n    opacity: 0;\n  }\n}</style>";
		m_bodySize = errorPage.size();
		buildHeaders();
		m_buffer += errorPage;
		m_done = true;
	}
	else
	{
		m_file = fopen(errorPagePath.c_str(), "r");
		m_headersMap["Content-Type"] = getContentType(errorPagePath);
		struct stat fileStat;
		if (stat(errorPagePath.c_str(), &fileStat) != 0)
		{
			m_statusCode = "500";
			return;
		}
		m_bodySize = fileStat.st_size;
		buildHeaders();
		m_done = true;
	}
}

void		Response::setLastSent(long sent)
{
	m_lastSent = sent;
}

std::string 		Response::peek(bool &done)
{
	// std::cout << "l/ast sent: " << m_lastSent << std::endl;
	m_cursor += m_lastSent;
	m_lastSent = 0;
	// std::cout << "cursor: " << m_cursor << std::endl;
	// std::cout << "buffer size: " << m_buffer.size() << std::endl;
	if (m_cursor >= (int)m_buffer.size())
	{
		// flush buffer
		m_buffer.clear();
		m_cursor = 0;
		if (m_done)
		{
			done = true;
			return ("");
		}
		if (m_file)
		{
			std::string ret;
			size_t read = fread(m_smolBuffer,1, RES_BUFFER_SIZE, m_file);
			if (ferror(m_file))
			{
				perror("fread");
			}
			if (read > 0)
				ret.assign(m_smolBuffer, read);
			else
				m_done = true;
			return ret;
		}
		else
			done = true;
		return ("");
	}
	else
	{
		return m_buffer.substr(m_cursor, m_buffer.size() - m_cursor);
	}
}

int			Response::getSd()
{
	return m_sd;
}

Response::Response(const Request &request)
{
	m_statusCode = "200";
	m_sd = request.getSd();
	m_request = request;
	m_cursor = 0;
	m_bodySize = 0;
	m_done = false;
	m_lastSent = 0;
	m_file = NULL;

	bool pass = true;
	if ((m_statusCode = request.getError()) != "200")
		pass = false;
	else if (request.getMethod() == "GET")
		pass = handleGet();
	else if (request.getMethod() == "POST")
		pass = handlePost();
	// else if (request.getMethod() == "DELETE")
	// 	pass = handleDelete();
	if (pass == false)
		setErrorPage();
}

Response::~Response() 
{
}

Response::Response(const Response &other)
{
	*this = other;
}

Response &Response::operator=(const Response &other)
{
	if (this != &other)
	{
		m_sd = other.m_sd;
		m_statusCode = other.m_statusCode;
		m_request = other.m_request;
		m_headersMap = other.m_headersMap;
		m_buffer = other.m_buffer;
		m_cursor = other.m_cursor;
		m_bodySize = other.m_bodySize;
		m_done = other.m_done;
		m_lastSent = other.m_lastSent;
		m_filePath = other.m_filePath;
		m_file = other.m_file;
	}
	return *this;
}

Response::Response() {}

std::ostream& operator<<(std::ostream& out, const Response& response)
{
	out << "Status_code" << response.m_statusCode << std::endl;
	out << "content-length" << response.getHeader("Content-Lenght") << std::endl;
	// out << response.m_headersMap.size() << std::endl;
	// for (std::map<std::string, std::string>::const_iterator it = response.m_headersMap.begin(); it != response.m_headersMap.end(); it++)
	// {
	// 	out << it->first << std::endl;
	// 	out << it->second << std::endl;
	// }
	// out << response.m_bodySize << std::endl;
	// out << response.m_buffer << std::endl;
	return out;
}