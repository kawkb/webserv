/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplexing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 23:35:05 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/23 02:12:45 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplexing.hpp"

Multiplexing::Multiplexing(void) {
	return ;
}

Multiplexing::~Multiplexing(void) {
	return ;
}

void	Multiplexing::runselect(fd_set *read_fds, fd_set *write_fds) {
	int		ret;
	int		fd;
	int		i;
	char buf[1024];

	ret = select(max_fds, read_fds, write_fds, NULL, NULL);
	if (ret == -1) {
		std::cout << "Error in select" << std::endl;
		exit(1);
	}
	i = 0;
	while (i < max_fds) {
		if (FD_ISSET(i, read_fds)) {
			std::cout << "Read fd: " << i << std::endl;
			read(i, buf, 1024);
		}
		if (FD_ISSET(i, write_fds)) {
			std::cout << "Write fd: " << i << std::endl;
			write(i, buf, 1024);
		}
		i++;
	}
}