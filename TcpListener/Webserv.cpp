/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 07:27:18 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/26 04:27:38 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void		Webserv::initiateMasterSockets(void)
{
	std::vector<int> masterPorts;
	for(std::vector<Server>::iterator i = m_servers.begin(); i != m_servers.end();i++)
	{
		if (std::find(masterPorts.begin(), masterPorts.end(), i->getPort()) == masterPorts.end())
		{
			masterPorts.push_back(i->getPort());
			m_tcplisteners.push_back(TcpListener(i->getPort()));
		}
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
	// set timeout for select
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
    if ((select(m_maxSd + 1, &m_readSet, &m_writeSet, NULL, &timeout) < 0))
		exit_failure("select error");
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
				int flags = fcntl(connection, F_GETFL, 0);
				flags |= O_NONBLOCK;
				fcntl(connection, F_SETFL, flags);
				FD_SET(connection, &m_readSetBackup);
				m_sds.push_back(connection);
			}
		}
	}
}
std::vector<Request>::iterator		Webserv::getRequest(int sd)
{
	for (std::vector<Request>::iterator it = m_requests.begin(); it != m_requests.end(); ++it)
	{
		if(it->getSd() == sd)
			return(it);
	}
	return(m_requests.end());
}

void		Webserv::handleRequest(void)
{
	char buf[4096];
	std::vector<int>::iterator i = m_sds.begin();
	while (i != m_sds.end())
	{
		if(FD_ISSET(*i, &m_readSet))
		{
			std::vector<Request>::iterator it = getRequest(*i);
			if (it == m_requests.end())
			{
				m_requests.push_back(Request(*i));
				i++;
			}
			else
			{
				int rec = recv(*i, &buf, 4096, 0); // check recv error
				if (rec == 0 || rec == -1)
				{
					close(*i);
					FD_CLR(*i, &m_readSetBackup);
					i = m_sds.erase(i);
					std::string reqfilename = it->getFilePath();
					if (!reqfilename.empty())
						unlink(reqfilename.c_str());
					m_requests.erase(it);
				}
				else
				{
					it->parse(m_servers, buf, rec);	
					i++;
				}
			}
		}
		else
			i++;
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
				if (sent <= 0)
					goto erase;
				i->setLastSent(sent);
				++i;
			}
			else
			{
				erase:
					fclose(i->getFile());
					FD_CLR(i->getSd(), &m_writeSetBackup);
					int	reqBodyFd = i->getReqBodyFd();
					if (reqBodyFd != -1)
						close(reqBodyFd);
					std::string reqFilename = i->getReqFilename();
					if (!reqFilename.empty())
						unlink(reqFilename.c_str());
					if (i->getKeepAlive())
						FD_SET(i->getSd(), &m_readSetBackup);
					else
					{
						close(i->getSd());
						m_sds.erase(std::find(m_sds.begin(), m_sds.end(), i->getSd()));
					}
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