#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <map>


int main()
{
    std::map<std::string, std::string> m_headers;
  
    m_headers.insert(std::pair<std::string, std::string>("User-Agent", "PostmanRuntime/7.29.2"));
    m_headers.insert(std::pair<std::string, std::string>("Accept", "*/*"));
    m_headers.insert(std::pair<std::string, std::string>("Postman-Token", "0a161d60-994b-4f9a-8afe-6195dc24920f"));
    m_headers.insert(std::pair<std::string, std::string>("Host", "localhost:8080"));
    m_headers.insert(std::pair<std::string, std::string>("Accept-Encoding", "gzip, deflate, br"));
    m_headers.insert(std::pair<std::string, std::string>("Connection", "keep-alive"));
    for (std::map<std::string, std::string>::iterator i = m_headers.begin(); i != m_headers.end(); ++i)
    {
        std::cout << i->first << ":" << i->second << std::endl;
    }
}

// GET / HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f
// Host: localhost:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive