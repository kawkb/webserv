/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:52:09 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/22 21:55:22 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	exit_failure(std::string str)
{
	perror(strerror(errno));
	std::cout << "\033[1;31m" << str << "\033[0m" << std::endl;
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

void	accept_connections(std::vector<TcpListener> &tcplisteners, fd_set &read_set, std::vector<Request> &requests)
{
	int		connection;
	for (std::vector<TcpListener>::iterator i = tcplisteners.begin(); i != tcplisteners.end(); ++i)
	{
		if (FD_ISSET(i->getMaster(), &read_set))
		{
			if ((connection = accept(i->getMaster(), NULL, NULL)) < 0)
				exit_failure("Failed to accept connection. errno: ");
			else
			{
				// make non blocking
				int flags = fcntl(connection, F_GETFL, 0);
				flags |= O_NONBLOCK;
				fcntl(connection, F_SETFL, flags);
				requests.push_back(Request(connection));
			}
		}
	}
}

void		set_master_sockets(std::vector<TcpListener> &tcplisteners, fd_set &read_set, int &max_sd)
{
	max_sd = -1;
	FD_ZERO(&read_set);
	for (std::vector<TcpListener>::iterator i = tcplisteners.begin(); i != tcplisteners.end(); ++i)
	{
		FD_SET(i->getMaster(), &read_set);
		if (i->getMaster() > max_sd)
			max_sd = i->getMaster();
	}
}

void	set_clients_sockets(std::vector<Request> &requests, std::vector<Response> &responses, fd_set &read_set, fd_set &write_set, int &max_sd)
{
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{
		FD_SET(i->getSd(), &read_set);
		// FD_SET(i->getBody(), &write_set);
		if (i->getSd() > max_sd)
			max_sd = i->getSd();
		// if (i->getBody() > max_sd)
			// max_sd = i->getBody();
	}
	for (std::vector<Response>::iterator i = responses.begin(); i != responses.end(); ++i)
	{
		FD_SET(i->getSd(), &write_set);
		if (i->getSd() > max_sd)
			max_sd = i->getSd();
	}
}

void		handle_requests(const std::vector<Server> &servers, fd_set &read_set, std::vector<Request> &requests)
{
	char buf[4096]; 
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{
		if (FD_ISSET(i->getSd(), &read_set))
		{
			int rec = recv(i->getSd(), &buf, 4096, 0);
			i->parse(servers, buf, rec);
		}
	}	
}

void	handle_responses(fd_set &write_set, std::vector<Request> &requests, std::vector<Response> &responses)
{
	std::vector<std::vector<Request>::iterator> tmp;
	for (std::vector<Request>::iterator i = requests.begin(); i != requests.end(); ++i)
	{
		if (i->getStatus() != "")
		{
			responses.push_back(Response(*i));
			tmp.push_back(i);
		}
	}
	for (std::vector<std::vector<Request>::iterator>::iterator i = tmp.begin(); i != tmp.end(); ++i)
		requests.erase(*i);
	std::vector<std::vector<Response>::iterator> tmp2;
	for(std::vector<Response>::iterator i = responses.begin(); i != responses.end(); ++i)
	{
		if (FD_ISSET(i->getSd(), &write_set))
		{
			// std::cout << *i << std::endl;
			bool done = false;
			std::string to_send = i->peek(done);
			if (!done)
			{
				ssize_t sent = send(i->getSd(), to_send.c_str(), to_send.size(), 0);
				if (sent < 0)
					exit_failure("FATAL : Failed to send response. errno: ");
				i->setLastSent(sent);
			}
			else
			{
				tmp2.push_back(i);
			}
		}
	}
	for (std::vector<std::vector<Response>::iterator>::iterator i = tmp2.begin(); i != tmp2.end(); ++i)
	{
		int tmp = (*i)->getSd();
		responses.erase(*i);
		close(tmp);
		FD_CLR(tmp, &write_set);
	}
}

void     run_server(std::vector<Server> &servers, std::vector<TcpListener> &tcplisteners)
{
	std::vector<Request>	requests;
	std::vector<Response>	responses;
	fd_set					read_set, write_set, read_set_backup;
	int						max_sd, max_sd_backup;
	initiate_master_sockets(servers, tcplisteners);
	FD_ZERO(&write_set);
	FD_ZERO(&read_set);
	set_master_sockets(tcplisteners, read_set_backup, max_sd);
	max_sd_backup = max_sd;
	while (TRUE)
	{
		read_set = read_set_backup;
		max_sd = max_sd_backup;
		set_clients_sockets(requests, responses, read_set, write_set, max_sd);
		if ((select(max_sd + 1, &read_set, &write_set, NULL, NULL) < 0))
			exit_failure("select error");
		accept_connections(tcplisteners, read_set, requests);
		handle_requests(servers, read_set, requests);
		handle_responses(write_set, requests, responses);
	}
	// return (0);
}
