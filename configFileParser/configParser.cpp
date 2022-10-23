/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 22:02:44 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/09/25 17:34:39 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int     check_server(std::vector<Server> &server)
{
	for (std::vector<Server>::iterator i = server.begin(); i !=server.end();i++)
	{
		std::vector<Server>::iterator k = i + 1;
		if ( i->getPort() < 0 || i->getPort() > 65536)
		{
			std::cerr << "\033[1;31mERROR: port out of range. \033[0m"<< std::endl;
			return(1);
		}
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

int     parse_config_file(char *av, std::vector<Server> &server)
{
	std::ifstream	myfile;
	std::string  line;

	myfile.open(av);
	if (myfile.is_open())
	{
		while (getline(myfile,line))
		{
			if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
            	continue;
			else if (line == "server")
			{
				Server holder;
				if (holder.parse(myfile))
				{
					myfile.close();
					return(1);
				}
				server.push_back(holder);
			}
			else
			{
				std::cout << "\033[1;31mConfigfile Error: \033[0m" << line << "."<< std::endl;
				myfile.close();
				return(1);
			}
		}
		myfile.close();
	}
	if (check_server(server))
		return(1);
	return(0);
}