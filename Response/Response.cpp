/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/13 22:50:20 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// add to utils later
template<class T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string Response::getCodeString(std::string code)
{
	if (code == "200")
		return "OK";
	if (code == "201")
		return "Created";
	if (code == "202")
		return "Accepted";
	if (code == "204")
		return "No Content";
	if (code == "301")
		return "Moved Permanently";
	if (code == "302")
		return "Found";
	if (code == "304")
		return "Not Modified";
	if (code == "400")
		return "Bad Request";
	if (code == "401")
		return "Unauthorized";
	if (code == "403")
		return "Forbidden";
	if (code == "404")
		return "Not Found";
	if (code == "405")
		return "Method Not Allowed";
	if (code == "406")
		return "Not Acceptable";
	if (code == "408")
		return "Request Timeout";
	if (code == "413")
		return "Payload Too Large";
	if (code == "414")
		return "URI Too Long";
	if (code == "415")
		return "Unsupported Media Type";
	if (code == "500")
		return "Internal Server Error";
	if (code == "501")
		return "Not Implemented";
	if (code == "502")
		return "Bad Gateway";
	if (code == "503")
		return "Service Unavailable";
	if (code == "505")
		return "HTTP Version Not Supported";
	return "Unknown";
}

std::string	Response::generateAutoIndex(std::string path)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	std::string authIndexHtml;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				if (ent->d_type == DT_DIR)
					dirs.push_back(ent->d_name);
				else
					files.push_back(ent->d_name);
			}
		}
		closedir (dir);
	}
	else
	{
		return ("");
	}
	std::sort(files.begin(), files.end());
	std::sort(dirs.begin(), dirs.end());
	authIndexHtml += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " + path + "</title>\n</head>\n<body>\n<h1>Index of " + path + "</h1>\n<hr>\n<table>\n<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>\n";
	authIndexHtml += "<tr><td><a href=\"../\">../</a></td><td></td><td></td></tr>\n";
	for (std::vector<std::string>::iterator i = dirs.begin(); i != dirs.end(); i++)
	{
		std::string folderpath = path + "/" + *i;
		struct stat attrib;
		if (stat(folderpath.c_str(), &attrib) < 0)
			return "";
		std::string lastModified = ctime(&attrib.st_mtime);
		lastModified.erase(lastModified.size() - 1);
		authIndexHtml += "<tr><td><a href=\"" + *i + "/\">" + *i + "/</a></td><td>" + lastModified + "</td><td></td></tr>\n";
	}
	for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::string filePath = path + "/" + *i;
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) < 0)
			return ("");
		std::string lastModified = ctime(&fileStat.st_mtime);
		lastModified.erase(lastModified.size() - 1);
		authIndexHtml += "<tr><td><a href=\"" + *i + "\">" + *i + "</a></td><td>" + lastModified + "</td><td>" + toString(fileStat.st_size) + "</td></tr>\n";
	}
	authIndexHtml += "</table>\n<hr>\n</body>\n</html>";
	authIndexHtml += "<style>\nbody {\nfont-family: sans-serif;\n}\n\nh1 {\nfont-size: 1.5em;\n}\n\nhr {\nmargin: 1em 0;\n}\n\ntable {\nborder-collapse: collapse;\nwidth: 100%;\n}\n\ntd, th {\nborder: 1px solid #ccc;\npadding: 0.5em;\n}\n\nth {\nbackground: #eee;\n}\n\ntr:nth-child(even) {\nbackground: #f8f8f8;\n}\n</style>";
	return authIndexHtml;
}

int Response::methodAllowed(const Request &request)
{
	for (std::vector<std::string>::iterator i = m_location.getMethod().begin(); i != m_location.getMethod().end(); i++)
		if (*i == request.getMethod())
			return (0);
	return (1);
}

bool Response::matchLocation(const Request &request)
{
	std::vector<Location> locations = m_server.getLocation();
	for (std::vector<Location>::iterator i = locations.begin(); i != locations.end(); i++)
	{
		std::string sub = request.getUri().substr(0, i->getPath().size());
		if (i->getPath() == sub)
		{
			m_location = *i;
			return (0);
		}
	}
	return (1);

	if (m_location.getRedirection().first != "")
		return ("301 Moved Permanently\n");
}

bool Response::matchServer(const std::vector<Server> &servers, const Request &request)
{
	std::string serverName, port, host;
	host = request.getHeader("Host");
	size_t pos = host.find(":");
	if (pos != std::string::npos)
	{
		serverName = host.substr(0, pos);
		port = host.substr(pos + 1);
	}
	bool first = false;
	for (std::vector<Server>::const_iterator i = servers.begin(); i != servers.end(); i++)
	{
		if (i->getPort() == atoi(port.c_str()))
		{
			if (first == false)
			{
				m_server = *i;
				first = true;
			}
			if (i->getName() == serverName)
			{
				m_server = *i;
				break;
			}
		}
	}
	if (request.getHeader("Content_length") > m_server.getMaxBodySize())
	{
		m_statusCode = 413;
		return (false);
	}
	return (true);
}

Response::~Response() {}
Response::Response() {}

std::string readFile(std::string path)
{
	std::ifstream file(path.c_str());
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

bool Response::handleGet(const Request &req)
{
	// location should never end with a slash
	// path should always end with a slash
	std::string uri = req.getUri();
	std::string rest = uri.substr(m_location.getPath().size());
	if (rest[0] == '/')
		rest = rest.substr(1);
	if (rest == "")
		rest = m_location.getIndex();
	std::string path = m_location.getRoot() + rest;
	// check if path is a directory
	struct stat path_stat;
	if (stat(path.c_str(), &path_stat) < 0)
	{
		m_statusCode = "404";
		return (false);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		if (path[path.size() - 1] != '/')
		{
			m_statusCode = "301";
			m_response = "HTTP/1.1 " + m_statusCode + " " + getCodeString(m_statusCode) + "\r\n";
			m_response += "Location: " + req.getHeader("host") + m_location.getPath() + rest + "/\r\n";
			return (false);
		}
		if (m_location.getAutoIndex() == 1)
		{
			m_statusCode = "200";
			m_response = "HTTP/1.1 " + m_statusCode + " " + getCodeString(m_statusCode) + "\r\n";
			m_response += "Content-Type: text/html\r\n";
			m_body = generateAutoIndex(path);
			m_response += "Content-Length: " + toString(m_body.size())  + "\r\n";
			m_response += "\r\n";
			m_response += m_body;
			return (true);
		}
		else
		{
			m_statusCode = "403";
			return (false);
		}
	}
	else
	{
		m_statusCode = "200";
		m_response = "HTTP/1.1 " + m_statusCode + " " + getCodeString(m_statusCode) + "\r\n";
		m_response += "Content-Type: " + getContentType(path) + "\r\n";
		m_response += "Content-Length: " + toString(path_stat.st_size) + "\r\n";
		m_response += "\r\n";
		m_body = readFile(path);
		m_response += m_body;
		return (true);
	}
}

void Response::setErrorPage()
{
	m_response = "HTTP/1.1 " + m_statusCode + " " + getCodeString(m_statusCode) + "\r\n";
	m_response += "Content-Type: text/html; charset=UTF-8\r\n";
	std::string errorPagePath = m_server.getErrorPage(m_statusCode);
	if (errorPagePath == "")
	{
		// fall back to default error page
		// m_response += "Content-Length: " + toString(m_errorPage.size()) + "\r\n";
		// m_response += "\r\n";
		// m_response += m_errorPage;
	}
	else
	{
		std::ifstream file;
		file.open(errorPagePath.c_str());
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		m_response += "Content-Length: " + toString(content.size()) + "\r\n";
		m_response += "\r\n";
		m_response += content;
	}
}

bool Response::isWellFormed(const Request &request)
{
	if ((request.getMethod() != "GET" && request.getMethod() != "POST" && request.getMethod() != "DELETE"))
	{
		m_statusCode = "501";
		return (false);
	}
	if (request.getVersion() != "HTTP/1.1")
	{
		m_statusCode = "505";
		return (false);
	}
	if (request.getHeader("Transfer-Encoding") != "chunked")
	{
		m_statusCode = "501";
		return (false);
	}
	if (request.getMethod() == "POST" && request.getHeader("Transfer-Encoding").empty() && request.getHeader("Content-Length").empty())
	{
		m_statusCode = "400";
		return (false);
	}
	if (request.getUri().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
	{
		m_statusCode = "400";
		return (false);
	}
	if (request.getUri().length() > 2048)
	{
		m_statusCode = "414";
		return (false);
	}
	else
		return (true);
}

bool Response::handlePost(const Request &req)
{
	const std::string upload_path = m_location.getUploadPath();
	if (upload_path == "")
	{
		// return 403
		m_response = "";
	}
	// handle CGI
	// ...
	// handle file upload
	else
	{
		// get what is after location in uri
		std::string uri = req.getUri();
		std::string location = m_location.getPath();
		std::string path = uri.substr(location.size());
		if 
	}
}

Response::Response(const Request &request, const std::vector<Server> &servers)
{
	bool pass = true;

	if (!isWellFormed(request))
		pass = false;
	else if (!matchServer(servers, request))
		pass = false;
	else if (!matchLocation(request))
		pass = false;
	else if (request.getMethod() == "GET")
		pass = handleGet(request);
	else if (request.getMethod() == "POST")
		pass = handlePost(request);
	else if (request.getMethod() == "DELETE")
		pass = handleDelete(request);
	if (pass == false)
		setErrorPage();
}