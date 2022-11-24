/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 22:02:44 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 08:42:00 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


int     order_servers(std::vector<Server> &server)
{
	for (std::vector<Server>::iterator i = server.begin(); i !=server.end();i++)
	{
		std::vector<Server>::iterator k = i + 1;
		for (std::vector<Server>::iterator j = i + 1; j != server.end(); j++)
		{
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
			if (((line.empty() || line.find_first_not_of(" \t") == std::string::npos) || line.erase(0, line.find_first_not_of(" \t")).rfind("//", 0) == 0) && lineCount++)
            	continue;
			else if (line == "server" && lineCount++)
				server.push_back(Server(myfile, lineCount));
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
	order_servers(server);
}
