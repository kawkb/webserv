#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../configFileParser/Server.hpp"
std::vector<std::string>    tokenize(std::string line);

class Request
{
    private:
        int         m_sd;
        std::vector<char>                   m_requestBuffer;
        std::string                         m_method;
        std::string                         m_uri;
        std::string                         m_version;
        std::string                         m_headersBuffer;
        std::map<std::string, std::string>  m_headers;
        std::vector<char>                   m_body;
        int                                 m_firstLine;
        int                                 m_headerStart;
        int                                 m_bodyStart;
    public:
        int         getSd(void) const;
        Server      getServer(void)const;
        std::string getMethod(void)const;
        std::string getHeaders(void)const;
        void        parse(const char *buf, int bufSize);
        std::string isWellFormed(void);
                    Request(void);
                    Request(int sd);
                    ~Request(void);
};
std::ostream& operator<<(std::ostream& os, Request request);

