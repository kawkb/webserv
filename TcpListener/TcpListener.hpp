/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 15:27:06 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/26 01:24:06 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"



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
