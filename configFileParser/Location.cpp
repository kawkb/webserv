/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdrissi- <kdrissi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 19:04:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/21 06:52:43 by kdrissi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int     Location::parse(std::ifstream &myfile, size_t &lineCount)
{
	std::string					line;
	std::vector<std::string>	token;
	bool		                isLocation = 0;
	while (getline(myfile, line))
	{
        if ((line.empty() || line.find_first_not_of(" \t") == std::string::npos) && lineCount++)
            continue;
        else
            token = tokenize(line);
		int size = token.size();
		if (token[0] == "{" && size == 1 && !isLocation && lineCount++)
			isLocation = 1;
		else if (token[0] == "allow_methods" && size <= 4 && lineCount++ && isLocation && m_method.empty())
		{
            for( int i = 1; i < size; i++)
            {  
                if (token[i] == "GET" || token[i] == "POST" || token[i] == "DELETE" )
                    m_method.push_back(token[i]);
                else
                {
					std::cerr << "\033[1;31mConfigfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"";
                    return(1);
                }
            }
		}
		else if (token[0] == "redirection" && size == 2 && lineCount++ && isLocation && m_redirection== "")
            m_redirection = token[1];
		else if (token[0] == "root" && size == 2 && lineCount++ && isLocation && m_root == "")
			m_root = token[1];
		else if (token[0] == "index" && size == 2 && lineCount++ && isLocation && m_index == "")
			m_index = token[1];
		else if (token[0] == "upload_path" && size == 2 && lineCount++ && isLocation && m_uploadPath == "")
            m_uploadPath = token[1];
		else if (token[0] == "autoindex" && size == 2 && lineCount++ && isLocation && m_autoIndex == 0)
			m_autoIndex = token[1] == "on" ? 1 : 0;
		else if (token[0] == "}" && size == 1 && isLocation && lineCount++)
			return(0);
		else
		{
			std::cerr << "\033[1;31mConfigfile Error in Line  "+ std::to_string(lineCount)+ ": \""+line + "\"";
			return(1);
		}
	}
    std::cerr << "\033[1;31mConfigfile Error in Lineb "+ std::to_string(lineCount)+ ": \""+line + "\"";
    return(1);
}

// std::ostream& operator<<(std::ostream& out, Location location)
// {
// 	std::cout << std::endl << "==========Location============" << std::endl;
// 	std::vector<std::string> method = location.getMethod();
// 	out << "location path: "<< location.getPath() << std::endl;
// 	out << "location index: " << location.getIndex() << std::endl;
// 	out << "location uploadpath: " << location.getUploadPath() << std::endl;
// 	out << "location root: " << location.getRoot() << std::endl;
// 	out << "location redirection: " << location.getRedirection() <<std::endl;
// 	out << "location autoindex: " << location.getAutoIndex() << std::endl;
// 	for (long unsigned int i = 0; i < method.size(); i++)
// 	{
// 		out << "Method:" << " " <<method[i] << " ";
// 	}
// 	return out;
// }

void        				Location::setPath(std::string path){m_path = path;}
std::string 				Location::getRedirection(void) const{return(m_redirection);}
std::string     			Location::getPath(void) const{return(m_path);}
int							Location::getAutoIndex(void) const{return(m_autoIndex);}
std::string 				Location::getUploadPath(void) const{return(m_uploadPath);}
std::vector<std::string>	Location::getMethod(void) const{return(m_method);}
std::string     			Location::getRoot(void) const{return(m_root);}
std::string     			Location::getIndex(void) const{return(m_index);}

Location::Location()
{
	m_index 		= "";
	m_root 			= "";
	m_path			= "";
	m_uploadPath 	= "";
	m_autoIndex 	= 0;
	m_redirection = "";
}

Location::Location(const Location &cp){*this = cp;}

Location & Location::operator=(const Location &cp)
{
    m_index 		= cp.getIndex();
    m_root 			= cp.getRoot();
	m_method 		= cp.getMethod();
	m_path			= cp.getPath();
	m_redirection 	= cp.getRedirection();
    m_autoIndex 	= cp.getAutoIndex();
	m_uploadPath 	= cp.getUploadPath();
	return (*this);
}

Location::~Location(){}
