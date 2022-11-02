#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1




#pragma once


#include "../Location/Location.hpp"

// Create Server class with all nginx configuration parameters

class Server {
    public:
        Server();
        //~Server();
        void attach(const Location location);
        bool attach(std::string pair);
        void debug();
        void lessgo();
        void run();
        void _socket();
        void setName(std::string name);
        Location *findLocation(std::string path);
    private:
        std::unordered_map<std::string, std::string> keys;
        std::string _name;
        std::vector<Location> locations;
        struct sockaddr_in address;
        int server_fd;
        struct fd_set backup_write;
	    struct fd_set write_fds;
	    struct fd_set read_fds;
	    struct fd_set backup_read;
};

#endif