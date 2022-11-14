/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:27:06 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/04 01:09:03 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cerrno>
#include "../configFileParser/Server.hpp"

#define TRUE   1 
#define FALSE  0
#define MAX_CLIENT 1024

class TcpListener
{
	private:
		int 					m_master;
		sockaddr_in				m_sockAddr;
		int						m_port;
	public:
								TcpListener();
								~TcpListener();
								TcpListener(int port);
		int						getPort(void) const;
		int						getMaster(void) const;
		sockaddr_in				&getAddress(void);
}; 