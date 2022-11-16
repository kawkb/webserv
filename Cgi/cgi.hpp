/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ziyad <ziyad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 17:02:02 by ziyad             #+#    #+#             */
/*   Updated: 2022/11/15 17:29:13 by ziyad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Response/Response.hpp"

class Cgi
{
	private:
		Response &response;
	public:
		Cgi(Response &response);
		~Cgi();
		Cgi(const Response &response);
		FILE *serveCgi();
		void setenvCgi();
		Response getResponse();
		int getTest();
		
};