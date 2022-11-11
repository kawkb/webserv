#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main()
{
    std::string m_headers = "helllo Host: localhost:8080\nfuck my life ";
    size_t pos = m_headers.find("Host: ");
    std::string host = m_headers.substr(pos);
    host = host.substr(host.find(" ") + 1, host.find('\n') - host.find(" ") - 1);
    std::cout << host << "|" << std::endl;
    std::string m_port = host.substr(0, host.find(':'));
    std::string m_serverName = host.substr(host.find(':') + 1);
    std::cout << m_port << std::endl << m_serverName;
    return(0);
}
// std::vector<std::string>    tokenize(std::string line)
// {
//     std::vector<std::string> tokens;
//     std::stringstream ss(line);
//     for(std::string line; ss >> line;)
//         tokens.push_back(line);
//     return(tokens);
// }

// int     main(void)
// {
//     size_t pos = 0;
//     std::string line = "";
//     std::string m_headers, m_body, m_method, m_version, m_uri;
//     int m_firstLine = 1;
//     int m_bodyStart = 0;
//     std::string m_request = " GET / HTTP/1.1\nUser-Agent: PostmanRuntime/7.29.2\nAccept: */*\nPostman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f\nHost: localhost:8080\nAccept-Encoding: gzip, deflate, br\nConnection: keep-alive\n\nhello world";
//     while(pos != std::string::npos)
//     {
//         pos = m_request.find("\n");
//         line = m_request.substr(0, pos + 1);
//         if (m_firstLine == 1)
//         {
//             std::vector<std::string> tokens;
//             tokens = tokenize(line);
//             m_firstLine = 0;
//             for(std::vector<std::string>::iterator i = tokens.begin(); i != tokens.end(); ++i)
//             {
//                 if(*i == "GET" || *i == "POST" || *i == "DELETE")
//                     m_method = *i;
//                 else if(*i == "HTTP/1.1")
//                     m_version = *i;
//                 else
//                     m_uri = *i;
//             }
//         }
//         else if (line.size() == 1)
//             m_bodyStart = 1;
//         else if (m_bodyStart == 1)
//             m_body += line;
//         else
//             m_headers += line; 
//         m_request.erase(0, pos + 1);
//     }

//     // std::cout << "method:: " << m_method << std::endl;
//     // std::cout << "version: " << m_version << std::endl;
//     // std::cout << "uri: " << m_uri << std::endl;
//     // std::cout << "headers: " << m_headers << std::endl;
//     // std::cout << "body: " << m_body << std::endl;
// }
// // GET / HTTP/1.1
// // User-Agent: PostmanRuntime/7.29.2
// // Accept: */*
// // Postman-Token: 0a161d60-994b-4f9a-8afe-6195dc24920f
// // Host: localhost:8080
// // Accept-Encoding: gzip, deflate, br
// // Connection: keep-alive