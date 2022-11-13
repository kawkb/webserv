#pragma once
#include "../Request/Request.hpp"
#include "../configFileParser/Server.hpp"
#include "../configFileParser/Location.hpp"

class Response
{
    private:
        int m_sd;
        std::string m_statusCode;
        std::string m_body;
        Server      m_server;
        Location    m_location;
        std::string m_serverName;
        std::string m_response;
        
    public:
        void    handleGet();
        void    handlePost();
        void    handleDelete();
        void    setErrorPage();
        int     methodAllowed(Request request);
        int     matchLocation(Request request);
        Server  matchServer(std::vector<Server> servers, std::string host);
                Response(Request request, std::vector<Server> servers);
                Response();
                ~Response();
};
