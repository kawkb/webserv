/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 15:14:24 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 08:38:24 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool bootstrap()
{
	std::string root = getenv("HOME");
	if (root == "")
		root = getenv("PWD");
	std::string path = root + "/.tmp";
	int f = mkdir(path.c_str(), 0777);
	if (f == -1)
	{
		if (errno != EEXIST)
			return false;
	}
	setenv("TMPDIR", path.c_str(), 1);
	return true;
}

int     main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	if (!bootstrap())
		return 1;
	if (ac != 2)
		exit_failure("Error: Bad Arguments!");
	std::vector<Server>			servers;
	parse_config_file(av[1], servers);
	Webserv webserv(servers);
	while (TRUE)
		webserv.run();
	return(0);
}
