#include <stdio.h>

#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

Server::Server()
{
    //std::cout << "We've got a server" << std::endl;
}


void Server::run()
{
	Response response("HTTP/1.1", "200", "OK");

	std::ifstream t("ErrorPages/index.html");
	std::stringstream _buffer;
	_buffer << t.rdbuf();
	response.appendHeader("Content-Length: " + std::to_string(_buffer.str().length()));
	response.appendHeader("Content-Type: text/html");
	response.addBody(_buffer.str());
	std::string hello = response.build();

	int ret;
	int new_fd;
	char buffer[30000] = {0};
    int i;
	int close_conn = FALSE;
	struct timeval	timeout;
	timeout.tv_sec  = 0;
	timeout.tv_usec = 1000;
	int addrlen = sizeof(address);

		read_fds = backup_read;
		write_fds = backup_write;
		ret = select(1024, &read_fds, &write_fds, NULL, &timeout);
		if (ret < 0)
		{
			perror("In select");
			exit(EXIT_FAILURE);
		}
		ret = 0;
		for (int i = 0; i < 1022; i++)
		{
			if (FD_ISSET(i, &read_fds))
			{			
				if (i == server_fd)
				{
					new_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					std::cout << "NEW_FD : " << new_fd << std::endl;
					if(new_fd > 0)
					{
						fcntl(new_fd, F_SETFL, O_NONBLOCK);
						FD_SET(new_fd, &backup_read);
					}
				}
				else
				{
					char buffer[30000] = {0};
        			int valread = recv(new_fd , buffer, 30000,0);
					
					// Too ugly, to refactor later
					if (valread > 0)
       				{
						std::string rt(buffer);
        			Request request(rt);
					Response response(request, *this);

					std::string res = response.build();

					ret =  send(i , res.c_str(), res.length(), 0);
					close_conn = TRUE;
					if (close_conn)
					{
						//request.debug();
						close(i);
						FD_CLR(i, &backup_read);
						close_conn = FALSE;
					}
					}
				}
			}
			else if (FD_ISSET(i, &write_fds))
			{
				ret =  send(i , hello.c_str(), hello.length(), 0);
				if (ret <= 0)
				{
					exit(EXIT_FAILURE);
				}
				FD_CLR(i, &backup_write);
				close(i);
				std::cout << "SEND_RET : " << i << "|" << ret << std::endl;
			}
		}
}

void Server::debug()
{
    std::cout << std::endl << "Server " << _name << " config:" << std::endl; 

    std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    //Iterate over the map using iterator
    while(it != keys.end())
    {
        std::cout << it->first << " :: " << it->second<<std::endl;
        it++;
    }

    std::vector<Location>::iterator ptr;
    for (ptr = locations.begin(); ptr != locations.end(); ++ptr)
        ptr->debug();

    /*std::cout << "Locations config:" << std::endl; 
    for (auto it = begin(locations); it != end(locations); ++it)
        it->debug();*/
}

bool Server::attach(std::string pair)
{
    std::vector<std::string> pairs = split(pair, ':');
    if (!pairs.empty())
        keys[pairs[0]] = pairs[1];
    else
       std::cout << "This shit so empty" << std::endl;
    return 1;
}

void Server::_socket()
{
	int opt = 1;
	int close_conn = FALSE;
	FD_ZERO(&backup_read);
	FD_ZERO(&backup_write);
	FD_ZERO(&write_fds);
	FD_ZERO(&read_fds);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(keys["server_name"].c_str());
    address.sin_port = htons(std::stoi(keys["listen"]));
    
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
    
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("Sockopt error");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("Sockopt error");
		exit(EXIT_FAILURE);
	}
			 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

	struct timeval	timeout;
	timeout.tv_sec  = 1;
	timeout.tv_usec = 0;
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	FD_SET(server_fd, &backup_read); //loop over all servers not just one
}


void Server::setName(std::string name)
{
    _name = name; 
}


void Server::attach(const Location location)
{
   locations.push_back(location);
}

Location *Server::findLocation(std::string path)
{
	std::vector<Location>::iterator ptr;
    for (ptr = locations.begin(); ptr != locations.end(); ++ptr)
	{
		std::cout << "Trying to find if " << path << " Belongs to location " << ptr->getPath() << std::endl;
		if (path.find(ptr->getPath().c_str(), 0, ptr->getPath().length()) != std::string::npos)
			return &(*ptr);
	}
	return NULL;
}