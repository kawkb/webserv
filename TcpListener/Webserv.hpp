/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 07:23:56 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/24 15:07:59 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
class Webserv
{
    private:
        std::vector<Request>	    m_requests;
    	std::vector<Response>	    m_responses;
        fd_set					    m_readSet;
        fd_set                      m_writeSet;
        fd_set                      m_readSetBackup;
    	int						    m_maxSd;
        int                         m_maxSdBackup;
        std::vector<Server>         m_servers;
        std::vector<TcpListener>    m_tcplisteners;
		std::vector<int>			m_connections;
    public:
        void                         initiateMasterSockets(void);
        void	                    setMasterSockets(void);
        void                        setFds();
        void                        multiplex(void);
        void                        acceptConnection(void);
        void	                    handleResponse(void);
        void		                handleRequest(void);
        
        void                        run(void);
                                    Webserv(void);
                                    Webserv(std::vector<Server> &server);
                                    ~Webserv(void);
};
