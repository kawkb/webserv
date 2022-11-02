#include "Request.hpp"

Request::Request()
{
    //std::cout << "We've got a Request" << std::endl;
}

Request::Request(std::string rt)
{
    std::stringstream to_stream(rt);
	std::vector<std::string> cgi_line;
    std::string line;
    std::getline(to_stream, line);
    line.resize(line.size() - 1);
	//Request req;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    if ((pos = rt.find("\r\n\r\n")) != std::string::npos) // has body check
        keys["body"] = rt.substr(pos + 4);

    pos = line.find(' ', prev);
    keys["reqtype"] = line.substr(0, pos - prev);
    prev = pos + 1;

    pos = line.find(' ', prev);
    keys["path"] = line.substr(prev, pos - prev);
    prev = pos + 1;
	
    pos = line.find(' ', prev);
    keys["vers"] = line.substr(prev);

    std::cout << "Received a <<" << keys["reqtype"] << ">> 	request, to path <<" << keys["path"] << ">> on http version <<" << keys["vers"] << ">>" << std::endl;
    
    //std::cout << "Request body <<" << keys["body"] << ">>" << std::endl;

    while (std::getline(to_stream, line))
    {
        line.resize(line.size() - 1);
        pos = line.find(':');
        if (pos != -1)
        {
            //std::cout << line.substr(0, pos) << " :: " << line.substr(pos + 2) << std::endl; 
            keys[line.substr(0, pos)] = line.substr(pos + 2);
        }
    }

    if (keys["Transfer-Encoding"].find("chunked") != std::string::npos)
    {
        std::cout << "Shit this body is chunked" << std::endl;
        std::string decoded_body;
        int length = 0;
        unsigned long chunk_size = strtol(keys["body"].c_str(), NULL, 16);
        while (chunk_size > 0)
        {
            length = keys["body"].find("\r\n", length) + 2;
            decoded_body += keys["body"].substr(length, chunk_size);
            length += chunk_size + 2;
            keys["body"] = keys["body"][length];
            chunk_size = strtol(keys["body"].c_str(), NULL, 16);
        }
        keys["body"] = decoded_body;
        keys["Transfer-Encoding"] = "";
        keys["Content-Length"] = length; // Later check if this is the right content size
    }
    std::cout << "Request body unchunked << " << keys["body"] << ">>" << std::endl;
}

std::string Request::getKey(std::string key)
{
    std::unordered_map<std::string, std::string>::iterator it = keys.begin();
    std::string searchedKey;

    while (it != keys.end())
    {
        if (it->first == key)
        {
            searchedKey = it->second;
            break;
        }
        it++;
    }
    return (searchedKey);
}

// Return a string of headers to print out on an HTML page
std::string Request::htmlHeaders()
{
    std::string string = "<ul>";

    std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    while(it != keys.end())
    {
        string += "<li><b>" + it->first + "</b>: " + it->second + "</li>";
        it++;
    }
    string += "</ul>";
    return (string);
}

void Request::debug()
{
    // To reuse later on

    std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    while(it != keys.end())
    {
        std::cout << it->first << " :: " << it->second<<std::endl;
        it++;
    }
}
