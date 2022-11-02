/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:06:36 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/26 06:04:18 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP
#include "../Webserv/Webserv.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

class cgi
{
	private:
		std::string _path;
		std::string _type;
		std::string _file;
		Request _req;
	public:
		cgi();
		cgi(std::string path, std::string type, Request req,std::string file);
		~cgi();
		void env();
};

#endif