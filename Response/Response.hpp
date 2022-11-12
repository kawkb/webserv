#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"

class Response
{
    private:
        int m_sd;
        std::string m_statusCode;
        std::string m_body;
        Server      m_server;
        std::string m_serverName;
        std::string m_response;
    public:
        void    handleGet();
        void    handlePost();
        void    handleDelete();
        void    setErrorPage();
        int     methodAllowed();
        int     matchLocation();
                Response(Request request, std::vector<Server> servers);
                Response();
                ~Response();
};
