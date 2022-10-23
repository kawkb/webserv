#pragma once
#include <string>

class Request
{
private:
    int         m_sd;
    std::string m_requestLine;
        //std::string m_method;
        //std::string m_path;
        //std::string m_uri;
        //std::string m_version;
    std::string m_headears;
        //std::string m_host;
        //std::string m_type;
        //std::string m_length;
    std::string m_body;
    

public:
    int     getSd(void) const;
    void    parse(char *buf);
    Request();
    Request(int sd);
    ~Request();
};


