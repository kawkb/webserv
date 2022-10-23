/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 19:05:05 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/09/24 14:36:26 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
std::vector<std::string>    tokenize(std::string line);

class Location
{
	private:
		std::string                 			m_index;
		std::string                 			m_root;
		std::vector<std::string>    			m_method;
		std::string								m_path;
		std::pair<std::string, std::string>   	m_redirection;
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
		std::pair<std::string, std::string> 	getRedirection(void) const;
		std::string                         	getUploadPath(void) const;
		void									setPath(std::string path);
		int										parse(std::ifstream &myfile);
		Location& 								operator=(const Location &cp);
};
std::ostream& operator<<(std::ostream& out, Location location);