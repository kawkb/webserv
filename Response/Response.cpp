/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 18:46:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/15 16:59:15 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


extern char** environ;
// add to utils later

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

std::string Response::generateAutoIndex(std::string path)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	std::string authIndexHtml;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL)
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


std::string getContentType(std::string filename)
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

bool Response::handleGet()
{
	// location should never end with a slash
	// path should always end with a slash
	std::string uri = req.getUri();
	std::string filename = uri.substr(.getPath().size());
	if (filename[0] == '/')
		filename = filename.substr(1);
	if (filename == "")
		filename = m_location.getIndex();
	std::string path = m_location.getRoot() + filename;
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
			m_response += "Location: " + req.getHeader("host") + m_location.getPath() + filename + "/\r\n";
			return (true);
		}
		if (m_location.getAutoIndex() == 1)
		{
			m_statusCode = "200";
			m_response = "HTTP/1.1 " + m_statusCode + " " + getCodeString(m_statusCode) + "\r\n";
			m_response += "Content-Type: text/html\r\n";
			m_body = generateAutoIndex(path);
			m_response += "Content-Length: " + toString(m_body.size()) + "\r\n";
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
		m_response += "Content-Type: " + getContentType(filename) + "\r\n";
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
	m_response += "Content-Type: text/html\r\n";
	std::string errorPagePath = m_server.getErrorPage(m_statusCode);
	if (errorPagePath == "")
	{
		m_body = "<html><head><title>" + m_statusCode + " " + getCodeString(m_statusCode) + "</title></head><body>";
		m_body += "<div class=\"base\">\n\t<div class=\"point\">></div>\n\t<h1><i>Http Error " + m_statusCode + ":</i> <br>";
		std::string errorString = getCodeString(m_statusCode);
		std::cout << errorString << std::endl;
		for (size_t i = 0; i < errorString.size(); i++)
		{
			if (errorString[i] == ' ')
				errorString.replace(i, 1, "<br>");
		}
		m_body += errorString + "</h1>\n</div></body></html>";
		m_body += "<style>body {\n  background-color: #23307e;\n}\nbody .base {\n  background-color: #23307e;\n  width: 100%;\n  height: 100vh;\n  display: flex;\n  align-items: center;\n  justify-content: center;\n  flex-direction: column;\n  position: relative;\n}\nbody .base .point {\n  font-family: \"Ubuntu\", sans-serif;\n  font-size: 10vw;\n  position: absolute;\n  top: 2vh;\n  color: #f2e9df;\n  left: 10vw;\n}\nbody .base .point:after {\n  content: \"_\";\n  animation: sparkle 0.5s infinite;\n  position: absolute;\n}\nbody .base h1 {\n  color: #ff3d57;\n  font-family: \"Ubuntu\", sans-serif;\n  text-transform: uppercase;\n  font-size: 6vw;\n  position: absolute;\n  top: 20vh;\n  left: 10vw;\n  -webkit-tap-highlight-color: rgba(255, 255, 255, 0);\n}\n@media only screen and (max-width: 992px) {\n  body .base h1 {\n    font-size: 10vw;\n  }\n}\nbody .base h1.glitch {\n  animation: clap 0.1s 5 forwards;\n}\n\n@keyframes sparkle {\n  0%, 100% {\n    opacity: 0;\n  }\n  50% {\n    opacity: 1;\n  }\n}\n@keyframes clap {\n  0%, 100% {\n    opacity: 1;\n  }\n  30% {\n    left: 11vw;\n    color: #f2e9df;\n  }\n  70% {\n    opacity: 0;\n  }\n}</style>";
		m_response += "Content-Length: " + toString(m_body.size()) + "\r\n";
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

bool Response::handlePost()
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

void	Response::increaseHeaderCursor(int cursor)
{
	m_headersCursor += cursor;
	if (m_headersCursor >= m_headers.size())
		m_headersSent = true;
}

void	Response::peekBody(char *buf, int *chunksize)
{
	const int restsize = m_bodySize - m_bodyCursor;
	if (restsize <= 0)
	{
		*chunksize = 0;
		return ;
	}
	*chunksize = min(restsize, BUFFER_SIZE);
	fseek(m_bodyFile, m_bodyCursor, SEEK_SET);
	fgets(buf, *chunksize, m_bodyFile);
	m_bodyCursor += *chunksize;
}

std::string Response::peekHeaders()
{
	return (m_headers.substr(m_headersCursor));
}

Response::Response(const Request &request)
{
	m_headersSent = false;
	m_headersCursor = 0;
	m_request = request;
	m_bodySent = false;
	m_bodyCursor = 0;

	bool pass = true;
	if ((m_statusCode = request.getError()) != "200")
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


void	setenvCGI(Request request)
{
	if (request.getMethod() == "GET")
	{
		std::string query = keys["query"];	
		if (query.size() != 0)
		{
			std::string c_size = std::to_string(keys["query"].length());
			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}		
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (request.getMethod() == "POST")
	{
		std::string content_type = request.getHeader("Content-Type");
		if (!(content_type.empty()))
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::string content_length = request.getHeader("Content-Length");
		if (!(content_length.empty()))
			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	}
	
	if (request.getHeader("cookie").size() != 0)
		setenv("HTTP_COOKIE", request.getHeader("cookie").c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", keys["query"].c_str(), 1);
	setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
	setenv("SCRIPT_FILENAME", keys["full_file_path"].c_str(), 1); // Parse file on request level
	setenv("SERVER_SOFTWARE", "Test", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

FILE    *Response::serveCgi(Request request)
{
	setenvCgi(request);
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
	{
		std::cout << "Error creating temporary file" << std::endl;
		return ;
	}
	int stdout_copy = dup(1);
	dup2(fileno(tempfile), 1);
	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (request.getMethod() == "POST")
		{
			int stdin_copy = dup(0);
			FILE *body = tmpfile();
			if (body == NULL)
			{
				std::cout << "Error creating temporary file" << std::endl;
				return ;
			}
			std::string body_str = request.keys["body"];
			fwrite(body_str.c_str(), 1, body_str.length(), body);
			rewind(body);
			dup2(fileno(body), 0);
		}
		if (keys["extension"] == ".php")
		{
			char *args[3];
			args[0] = (char *)keys["cgi_path"].c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
		else if (keys["extension"] == ".py")
		{
			char *args[3];
			std::string python = "/usr/bin/python";
			args[0] = (char *)python.c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
	}
	else
	{
		time_t t = time(NULL);
		while (time(NULL) - t < 5)
		{
			if (waitpid(pid, &status, WNOHANG) != 0)
				break;
		}
	}
	dup2(stdout_copy, 1);
	close(stdout_copy);
	rewind(tempfile);
	return (tempfile);
}

int		Response::getSd()
{
	return m_request.getSd();
}

Response::~Response() {}

Response::Response() {}