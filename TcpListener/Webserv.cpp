/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 07:27:18 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 18:41:48 by moerradi         ###   ########.fr       */
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
		if (i->getMaster() > m_maxSdBackup)
			m_maxSdBackup = i->getMaster();
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
			FILE*	body = it->getBody();
			if (body)
				fclose(body);
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
					close(i->getSd());
					fclose(i->getFile());
					i = m_responses.erase(i);
			}
		}
		else 
			++i;
	}
}

void    Webserv::run(void)
{
	std::cout << "max fd : " << m_maxSd << std::endl;
	// print all request sds
	std::cout << "request fds : " ;
	for (std::vector<Request>::iterator i = m_requests.begin(); i != m_requests.end(); ++i)
		std::cout << "request sd: " << i->getSd() << "-";
	std::cout << std::endl;
	// print all response sds
	std::cout << "response fds : ";
	for (std::vector<Response>::iterator i = m_responses.begin(); i != m_responses.end(); ++i)
		std::cout << "response sd: " << i->getSd() << "-";
	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;
    setFds();
	std::cout << "select hangs" << std::endl;
	multiplex();

	acceptConnection();

	handleRequest();

	handleResponse();
}

void	Webserv::setFds()
{
	// problem is here
	FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
	m_readSet = m_readSetBackup;
	m_maxSd = m_maxSdBackup;
	for (std::vector<Request>::iterator i = m_requests.begin(); i != m_requests.end(); ++i)
	{
		FD_SET(i->getSd(), &m_readSet);
		if (i->getSd() > m_maxSd)
			m_maxSd = i->getSd();
	}
	for (std::vector<Response>::iterator i = m_responses.begin(); i != m_responses.end(); ++i)
	{
		FD_SET(i->getSd(), &m_writeSet);
		if (i->getSd() > m_maxSd)
			m_maxSd = i->getSd();
	}
}

Webserv::Webserv(std::vector<Server> servers)
{
    m_servers = servers;
    initiateMasterSockets();
	m_maxSdBackup = -1;
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    FD_ZERO(&m_readSetBackup);
	setMasterSockets();
}

Webserv::~Webserv(){}

//add the keep alive feature;
// add the timeout 