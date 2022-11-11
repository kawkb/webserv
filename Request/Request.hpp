#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "../configFileParser/Server.hpp"
std::vector<std::string>    tokenize(std::string line);

class Request
{
    private:
        int         m_sd;
        std::string m_request;
        std::string m_method;
        std::string m_uri;
        std::string m_version;
        std::string m_headers;
        std::string m_serverName;
        std::string m_port;
        std::string m_body;
        Location    m_location;
        Server      m_server;
        int         m_firstLine;
        int         m_headerStart;
        int         m_bodyStart;

    public:
        int         getSd(void) const;
        Server      getServer(void)const;
        Server      matchServer(std::vector<Server> servers);
        int         matchLocation();
        std::string getMethod()const;
        int        Request::methodAllowed();
        std::string getHost(void) const;
        void        parse(const char *buf);
        std::string         isWellFormed();
                    Request();
                    Request(int sd);
                    ~Request();
};
