/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:52:09 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/16 00:24:46 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	exit_failure(std::string str)
{
	std::cout << str << std::endl;
	exit(EXIT_FAILURE);
}

int		initiate_master_sockets(std::vector<Server> &server, std::vector<TcpListener> &tcpListener)
{
	for(std::vector<Server>::iterator i = server.begin(); i != server.end();i++)
	{
		TcpListener holder(i->getPort());
		for(std::vector<Server>::iterator j = i + 1; j != server.end(); ++j)
		{
			if (j->getPort() == holder.getPort())
				i++;
			else
				break;
		}
		tcpListener.push_back(holder);
	}
	return(0); 
}

void	accept_connections(std::vector<TcpListener> &tcpListeners, fd_set &read_set, std::vector<Request> &requests, int &max_sd)
{
	int		connection;
	for (std::vector<TcpListener>::iterator i = tcpListeners.begin(); i != tcpListeners.end(); ++i)
	{
		if (FD_ISSET(i->getMaster(), &read_set))
		{
			if ((connection = accept(i->getMaster(), NULL, NULL)) < 0)
				exit_failure("Failed to accept connection. errno: ");
			else
				requests.push_back(Request(connection));
			if (connection > max_sd)
				max_sd = connection;
		}
	}
}

void		set_master_sockets(std::vector<TcpListener> &tcpListeners, fd_set &read_set, int &max_sd)
{
	max_sd = -1;
	FD_ZERO(&read_set);
	for (std::vector<TcpListener>::iterator i = tcpListeners.begin(); i != tcpListeners.end(); ++i)
	{
		FD_SET(i->getMaster(), &read_set);
		if (i->getMaster() > max_sd)
			max_sd = i->getMaster();
	}
}

void	set_clients_sockets(std::vector<Request> &requests, fd_set &read_set, int &max_sd)
{
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{
		FD_SET(i->getSd(), &read_set);
		if (i->getSd() > max_sd)
			max_sd = i->getSd();
	}
	for (std::vector<Response>::iterator i = response.begin(); i != response.end(); ++i)
	{
		FD_SET(i->getSd(), &write_set);
		if (i->getSd() > max_sd)
			max_sd = i->getSd();
	}
}

void		handle_requests(std::vector<TcpListener> &tcpListeners, fd_set &read_set, std::vector<Request> &requests)
{
	char buf[1024]; 
	(void)tcpListeners;
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{
		if (FD_ISSET(i->getSd(), &read_set))
		{
			int rec = recv(i->getSd(), &buf, 1024, 0);
			i->parse(buf, rec);
		}
	}
}

void	handle_responses(std::vector<Server> &servers, fd_set &write_set, std::vector<Request> &requests, std::vector<Response> &responses)
{
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{	
		if (i->isDone())
		{
			responses.push_back(Response(*i));
			requests.erase(i);
		}
	}
	for(std::vector<Response>::iterator i = responses.begin(); i != responses.end(); ++i)
	{
		if (FD_ISSET(i->getSd(), &write_set))
		{
			char buf[BUFFER_SIZE];
			long sendsize = BUFFER_SIZE;
			if (i->peek(buf, &sendsize))
			{
				send(i->getSd(), buf, sendsize, 0);
			}
			else
			{
				close(i->getSd());
				responses.erase(i);
			}
			
		}
	}
}

int     run_server(std::vector<Server> &servers, std::vector<TcpListener> &tcpListeners)
{
	std::vector<Request>	requests;
	std::vector<Response>	responses;
	fd_set					read_set, write_set, read_set_backup, write_set_backup;
	int						max_sd;
	initiate_master_sockets(servers, tcpListeners);
	FD_ZERO(&write_set);
	FD_ZERO(&read_set);
	set_master_sockets(tcpListeners, read_set_backup, max_sd);
	while (TRUE)
	{
		read_set = read_set_backup;
		write_set = write_set_backup;
		set_clients_sockets(requests, read_set, max_sd);
		if ((select(max_sd + 1, &read_set, &write_set, NULL, NULL) < 0))
			exit_failure("select error");
		accept_connections(tcpListeners, read_set, requests, max_sd);
		handle_requests(tcpListeners, read_set, requests);
		handle_responses(servers, write_set, requests, responses);
	}
	return (0);
}
