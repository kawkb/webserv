#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <map>
#include<dirent.h>
#include <fstream>
#include <sys/stat.h>
template<class T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
std::string getCodeString(std::string code)
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
int main(int arg, char **argv)
{
	std::string m_body;
	std::string m_statusCode = argv[1];
	m_body = "<html><head><title>" + m_statusCode + " " + getCodeString(m_statusCode) + "</title></head><body>";
	m_body += "<div class=\"base\">\n\t<div class=\"point\">></div>\n\t<h1><i>Http Error " + m_statusCode +":</i> <br>";
	std::string errorString = getCodeString(m_statusCode);
	std::cout << errorString << std::endl;
	for (size_t i = 0; i < errorString.size(); i++)
	{
		if (errorString[i] == ' ')
			errorString.replace(i, 1, "<br>");
	}
	m_body += errorString + "</h1>\n</div></body></html>";
	m_body += "<style>body {\n  background-color: #23307e;\n}\nbody .base {\n  background-color: #23307e;\n  width: 100%;\n  height: 100vh;\n  display: flex;\n  align-items: center;\n  justify-content: center;\n  flex-direction: column;\n  position: relative;\n}\nbody .base .point {\n  font-family: \"Ubuntu\", sans-serif;\n  font-size: 10vw;\n  position: absolute;\n  top: 2vh;\n  color: #f2e9df;\n  left: 10vw;\n}\nbody .base .point:after {\n  content: \"_\";\n  animation: sparkle 0.5s infinite;\n  position: absolute;\n}\nbody .base h1 {\n  color: #ff3d57;\n  font-family: \"Ubuntu\", sans-serif;\n  text-transform: uppercase;\n  font-size: 6vw;\n  position: absolute;\n  top: 20vh;\n  left: 10vw;\n  -webkit-tap-highlight-color: rgba(255, 255, 255, 0);\n}\n@media only screen and (max-width: 992px) {\n  body .base h1 {\n    font-size: 10vw;\n  }\n}\nbody .base h1.glitch {\n  animation: clap 0.1s 5 forwards;\n}\n\n@keyframes sparkle {\n  0%, 100% {\n    opacity: 0;\n  }\n  50% {\n    opacity: 1;\n  }\n}\n@keyframes clap {\n  0%, 100% {\n    opacity: 1;\n  }\n  30% {\n    left: 11vw;\n    color: #f2e9df;\n  }\n  70% {\n    opacity: 0;\n  }\n}</style>";
	// write body to file
	std::ofstream file;
	file.open("test.html");
	file << m_body;
	file.close();
	return 0;
}

// GET / HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f
// Host: localhost:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive


// hello bitches
// how are you
// what do i need to do today 
// so 