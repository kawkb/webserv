/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 22:02:44 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/22 02:14:50 by kdrissi-         ###   ########.fr       */
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

std::string resolvePath(std::string path)
{
    char *absolutePath = realpath(path.c_str(), NULL);
    if (absolutePath == NULL)
        return "";
    std::string result(absolutePath);
    free(absolutePath);
    return result;
}

int     check_server(std::vector<Server> &server)
{
	for (std::vector<Server>::iterator i = server.begin(); i !=server.end();i++)
	{
		std::vector<Server>::iterator k = i + 1;
		// if (i->getPort() < 0 || i->getPort() > 65536 || i->getName() == "" || i->getRoot() == "" || i->getMethod().empty() || resolvePath(i->getRoot()) == )
		// 	exit_failure("\033[1;31mERROR: somthing missing in Config File. \033[0m");
		// for (std::vector<Location>::iterator j = i->getLocation().begin(); j != i->getLocation().end(); j++)
		// {
		// 	if()
		// 	{
				
		// 	}
		// } 	 		
		// // server name, root, allowmethods; root always ends with /
		//location no / at the end; if upload path and somthing else error ; allormthods + root (ends wiwth /) always absolute;
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