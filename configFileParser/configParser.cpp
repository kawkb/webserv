/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 22:02:44 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/23 04:07:34 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


std::vector<std::string>    tokenize(std::string line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    for(std::string line; ss >> line;)
        tokens.push_back(line);
    return(tokens);
}
void	check_server_errors(Server &server)
{
		if (server.getPort() < 0 || server.getPort() > 65536)
			exit_failure("\033[1;31mConfig File Error: Port out of range. \033[0m");
		if(server.getName() == "")
			exit_failure("\033[1;31mConfig File Error: Missing server name. \033[0m");
		if(server.getMethod().empty())
			server.setMethod("GET");
		if(checkPath(server.getRoot()) != "")
			exit_failure("\033[1;31mConfig File Error: "+ checkPath(server.getRoot()) +" \033[0m");
		std::vector<Location> location = server.getLocation();
		for (std::vector<Location>::iterator it = location.begin(); it != location.end(); it++)
		{
			if(!it->getUploadPath().empty() && (!it->getRoot().empty() || !it->getIndex().empty() || !it->getMethod().empty()))
				exit_failure("\033[1;31mConfig File Error: Upload Path is specified in location no other directives allowed.\033[0m");
			if(!it->getUploadPath().empty() && !checkPath(it->getUploadPath()).empty())
				exit_failure("\033[1;31mConfig File Error: "+ checkPath(it->getRoot()) +" \033[0m");
			if(!it->getUploadPath().empty())
				it->setMethod("POST");
			if(it->getMethod().empty() && it->getUploadPath().empty())
				it->setMethod("GET");
			if(!it->getRoot().empty() && checkPath(it->getRoot()) != "")
				exit_failure("\033[1;31mConfig File Error: "+ checkPath(it->getRoot()) +" \033[0m");
		}
}
int     check_server(std::vector<Server> &server)
{
	for (std::vector<Server>::iterator i = server.begin(); i !=server.end();i++)
	{
		std::vector<Server>::iterator k = i + 1;
		check_server_errors(*i);
		for (std::vector<Server>::iterator j = i + 1; j != server.end(); j++)
		{
			check_server_errors(*j);
			if (i->getPort() == j->getPort())
			{
				if (i->getName() == j->getName())
				{
					std::cerr << "\033[1;31mERROR: duplicate server name. \033[0m"<< std::endl;
					return(1);
				}
				else if (j != k)
				{
					server.insert(k, *j);
					server.erase(j + 1);
					k++;
					i++;
				}
			}
		}
	}
	return(0);
}

void     parse_config_file(char *av, std::vector<Server> &server)
{
	std::ifstream	myfile;
	std::string		line;
	size_t			lineCount = 1;

	myfile.open(av);
	if (myfile.is_open())
	{
		while (getline(myfile,line))
		{
			if ((line.empty() || line.find_first_not_of(" \t") == std::string::npos) && lineCount++)
            	continue;
			else if (line == "server" && lineCount++)
			{
				Server holder;
				if (holder.parse(myfile, lineCount))
				{
					myfile.close();
					exit_failure("");
				}
				server.push_back(holder);
			}
			else
			{
				myfile.close();
				exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
			}
		}
		myfile.close();
	}
	else
		exit_failure("Error: open config file");
	check_server(server);
}