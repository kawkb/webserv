// Create Server class with all nginx configuration parameters
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

class Location {
    public:
        Location();
        bool attach(std::string pair);
        void debug();
        void setPath(std::string path);
        std::string getKey(std::string key);
        std::string getPath();
    private:
        std::unordered_map<std::string, std::string> keys;
        std::string _path;

        std::string server_index;
        std::string cgi_path;
        bool autoindex;
        std::string redirect_path;
        std::string server_path;
        std::string accepted_methods;
        std::string upload_path;   
        std::string default_file;
        std::string cgi_extension;     
};

std::vector<std::string> split(std::string string, char delimiter);

#endif