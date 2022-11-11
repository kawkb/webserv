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
        std::string m_type;
        std::string m_response;
    public:
        void    fill(Request request);
        void    handleGet();
        void    handlePost();
        void    handleDelete();
        void    setErrorPage();
                Response(Request request, std::vector<Server> servers);
                Response();
                ~Response();
};
