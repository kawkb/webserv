/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 19:05:05 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/23 02:08:22 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../webserv.hpp"
std::vector<std::string>    tokenize(std::string line);

class Location
{
	private:
		std::string                 			m_index;
		std::string                 			m_root;
		std::vector<std::string>    			m_method;
		std::string								m_path;
		std::string   							m_redirection;
		std::string                         	m_uploadPath; 
		int                         			m_autoIndex;
	public:
												Location();
												Location(const Location &cp);
												~Location();
		std::vector<std::string>        		getMethod(void) const;
		std::string                     		getRoot(void) const;
		std::string                     		getIndex(void) const;
		std::string                     		getPath(void) const;
		int										getAutoIndex(void) const;
		std::string							 	getRedirection(void) const;
		std::string                         	getUploadPath(void) const;
		void									setPath(std::string path);
		int										parse(std::ifstream &myfile, size_t &lineCount);
		void                        			setMethod(std::string method);
		Location& 								operator=(const Location &cp);
};
std::ostream& operator<<(std::ostream& out, Location location);