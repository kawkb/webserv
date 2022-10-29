#pragma once
#include <string>
#include <iostream>
#include <vector>
std::vector<std::string>    tokenize(std::string line);

class Request
{
private:
    int         m_sd;
    std::string m_request;
    std::string m_requestLine;
    int         m_firstLine;
    std::string m_method;
    std::string m_uri;
    std::string m_version;
    std::string m_headears;
        //std::vector<string> m_headers;
        //std::string m_host;
        //std::string m_type;
        //std::string m_length;
    std::string m_body;
    int         m_bodyStart;

public:
    int     getSd(void) const;
    void    parse(const char *buf);
    Request();
    Request(int sd);
    ~Request();
};


