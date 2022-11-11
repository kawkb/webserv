/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:27:33 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/04 01:08:40 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int		TcpListener::getPort(void) const
{
	return(m_port);
}


TcpListener::TcpListener(int port): m_port(port)
{
	int 		opt = 1;
	if ((m_master = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		exit_failure("Failed to create socket. errno: ");
	if (setsockopt(m_master, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		exit_failure("setsockopt failed");
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_addr.s_addr = INADDR_ANY;
	m_sockAddr.sin_port = htons(port);
	if (bind(m_master, (struct sockaddr*)&m_sockAddr, sizeof(sockaddr)) < 0) 
		exit_failure("Failed to bind to port. errno: ");
	if (listen(m_master, 1024) < 0)
		exit_failure("Failed to listen on socket. errno: ");
	// int flags = fcntl(m_master, F_GETFL, 0);
	// flags |= O_NONBLOCK;
	//TODO: implement further error handling for fcntl
	// fcntl(m_master, F_SETFL, flags);
	// m_server.push_back(server);
}

int TcpListener::getMaster(void) const
{
	return(m_master);
}
sockaddr_in		&TcpListener::getAddress(void)
{
	return(m_sockAddr);
}
TcpListener::TcpListener()
{
	
}

TcpListener::~TcpListener()
{
}
