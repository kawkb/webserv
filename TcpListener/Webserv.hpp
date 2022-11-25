/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 07:23:56 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/25 01:32:13 by moerradi         ###   ########.fr       */
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
		fd_set                      m_writeSetBackup;
		std::vector<int>			m_sds;
    	int						    m_maxSd;
        int                         m_maxMasterSd;
        std::vector<Server>         m_servers;
        std::vector<TcpListener>    m_tcplisteners;
    public:
        void                        		initiateMasterSockets(void);
        void	                    		setMasterSockets(void);
        void                        		setFds();
        void                        		multiplex(void);
        void                        		acceptConnection(void);
        void	                    		handleResponse(void);
        void		                		handleRequest(void);
		std::vector<Request>::iterator		getRequest(int sd);
        
        void                        run(void);
                                    Webserv(void);
                                    Webserv(std::vector<Server> &server);
                                    ~Webserv(void);
};
