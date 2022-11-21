/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:27:06 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/20 20:48:03 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"

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
