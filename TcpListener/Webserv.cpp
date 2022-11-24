/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 07:27:18 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/25 00:40:05 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void		Webserv::initiateMasterSockets(void)
{
	for(std::vector<Server>::iterator i = m_servers.begin(); i != m_servers.end();i++)
	{
		TcpListener holder(i->getPort());
		for(std::vector<Server>::iterator j = i + 1; j != m_servers.end(); ++j)
		{
			if (j->getPort() == holder.getPort())
				i++;
			else
				break;
		}
		m_tcplisteners.push_back(holder);
	}
}

void		Webserv::setMasterSockets(void)
{
	for (std::vector<TcpListener>::iterator i = m_tcplisteners.begin(); i != m_tcplisteners.end(); ++i)
	{
		FD_SET(i->getMaster(), &m_readSetBackup);
		if (i->getMaster() > m_maxMasterSd)
			m_maxMasterSd = i->getMaster();
	}
}

void    Webserv::multiplex(void)
{
    if ((select(m_maxSd + 1, &m_readSet, &m_writeSet, NULL, NULL) < 0))
		exit_failure("select error");
	std::cout << "select returned" << std::endl;
}

void    Webserv::acceptConnection(void)
{
	int		connection;
	for (std::vector<TcpListener>::iterator i = m_tcplisteners.begin(); i != m_tcplisteners.end(); ++i)
	{
		if (FD_ISSET(i->getMaster(), &m_readSet))
		{
			if ((connection = accept(i->getMaster(), NULL, NULL)) < 0)
				exit_failure("Failed to accept connection. errno: ");
			else
			{
				std::cout << "connection : " << connection << std::endl;
				int flags = fcntl(connection, F_GETFL, 0);
				flags |= O_NONBLOCK;
				fcntl(connection, F_SETFL, flags);
				FD_SET(connection, &m_readSetBackup);
				m_sds.push_back(connection);
				m_requests.push_back(Request(connection));
			}
		}
	}
}

void		Webserv::handleRequest(void)
{
	char buf[4096];
	std::vector<Request>::iterator it = m_requests.begin();

	while(it != m_requests.end())
	{
		if(FD_ISSET(it->getSd(), &m_readSet))
		{
			int rec = recv(it->getSd(), &buf, 4096, 0);
			// if (rec == 0)
			// {
			// 	close(it->getSd());
			// 	it = m_requests.erase(it);
			// }
			// if (rec == -1)
			// 	exit_failure("recv error");
			// else
				it->parse(m_servers, buf, rec);
		}
		it++;
	}
}

void	Webserv::handleResponse(void)
{
	std::vector<Request>::iterator it = m_requests.begin();
	while(it != m_requests.end())
	{
		if(it->getStatus() != "")
		{
			m_responses.push_back(Response(*it));
			FD_CLR(it->getSd(), &m_readSetBackup);
			FD_SET(it->getSd(), &m_writeSetBackup);
			it = m_requests.erase(it);
		}
		else 
			++it;
	}
	std::vector<Response>::iterator i = m_responses.begin();
	while(i != m_responses.end())
	{
		if (FD_ISSET(i->getSd(), &m_writeSet))
		{
			bool done = false;
			std::string to_send = i->peek(done);
			if (!done)
			{
				ssize_t sent = send(i->getSd(), to_send.c_str(), to_send.size(), 0);
				if (sent < 0)
					goto erase;
				i->setLastSent(sent);
				++i;
			}
			else
			{
				erase:
					fclose(i->getFile());
					FD_CLR(i->getSd(), &m_writeSetBackup);
					// if (i->getKeepAlive())
					// 	FD_SET(i->getSd(), &m_readSetBackup);
					// else
						close(i->getSd());
					m_sds.erase(std::find(m_sds.begin(), m_sds.end(), i->getSd()));
					i = m_responses.erase(i);
			}
		}
		else 
			++i;
	}
}

void    Webserv::run(void)
{
    setFds();
	multiplex();
	acceptConnection();
	handleRequest();
	handleResponse();
}

void	Webserv::setFds()
{
	// problem is here
	// FD_ZERO(&m_readSet);
    // FD_ZERO(&m_writeSet);
	m_readSet = m_readSetBackup;
	m_writeSet = m_writeSetBackup;
	m_maxSd = m_maxMasterSd;
	for(std::vector<int>::iterator i = m_sds.begin(); i != m_sds.end(); ++i)
	{
		if (*i > m_maxSd)
			m_maxSd = *i;
	}
}

Webserv::Webserv(std::vector<Server> &servers)
{
    m_servers = servers;
    initiateMasterSockets();
	m_maxMasterSd = -1;
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    FD_ZERO(&m_readSetBackup);
	FD_ZERO(&m_writeSetBackup);
	setMasterSockets();
	m_maxSd = m_maxMasterSd;
}

Webserv::~Webserv(){}

//add the keep alive feature;
// add the timeout 