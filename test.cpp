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

int main()
{
	std::string uri = "/Users/sa";
	std::string location = "/";
	// check if location ends with a slash
	std::string rest = uri.substr(location.size());
	if (rest[0] == '/')
		rest = rest.substr(1);
	if (rest == "")
		rest = "index.html";
	std::cout << rest;
	// std::string path = m_location.getRoot() + rest;
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