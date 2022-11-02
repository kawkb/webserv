/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexing.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 23:31:43 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/21 23:37:40 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv/Webserv.hpp"

class Multiplexing {

	public:
		Multiplexing(void);
		~Multiplexing(void);
		void addfdtoselect(int fd, fd_set *fds);
		void runselect(fd_set *read_fds, fd_set *write_fd);
	private:
		int max_fds;
		fd_set read_fds;
		fd_set write_fds;
};