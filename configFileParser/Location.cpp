/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 19:04:57 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/26 03:57:20 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void     Location::parse(std::ifstream &myfile, size_t &lineCount)
{
	std::string					line;
	std::vector<std::string>	token;
	bool		                isLocation = 0;
	while (getline(myfile, line))
	{
        if ((line.empty() || line.find_first_not_of(" \t") == std::string::npos || line.erase(0, line.find_first_not_of(" \t")).rfind("#", 0) == 0)&& lineCount++)
            continue;
        else
            token = tokenize(line);
		int size = token.size();
		if (token[0] == "{" && size == 1 && !isLocation && lineCount++)
			isLocation = 1;
		else if (token[0] == "allow_methods" && size <= 4 && lineCount++ && isLocation && m_method.empty())
		{
			m_gotMethod = 1;
            for( int i = 1; i < size; i++)
            {  
                if (token[i] == "GET" || token[i] == "POST" || token[i] == "DELETE" )
                    m_method.push_back(token[i]);
                else
                {
                    myfile.close();
                    exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
                }
            }
		}
		else if (token[0] == "redirection" && size == 2 && lineCount++ && isLocation && m_redirection== "")
            m_redirection = token[1];
		else if (token[0] == "root" && size == 2 && lineCount++ && isLocation && m_root == "")
		{
			m_root  = token[1];
            if(*m_root.rbegin() != '/')
		        m_root.append("/");
		}
		else if (token[0] == "index" && size == 2 && lineCount++ && isLocation && m_index == "")
			m_index = token[1];
		else if (token[0] == "upload_path" && size == 2 && lineCount++ && isLocation && m_uploadPath == "")
		{
			m_uploadPath  = token[1];
            if(*m_uploadPath.rbegin() != '/')
		        m_uploadPath.append("/");
		}
		else if (token[0] == "autoindex" && size == 2 && lineCount++ && isLocation && m_autoIndex == -1)
			m_autoIndex = token[1] == "on" ? 1 : 0;
		else if (token[0] == "}" && size == 1 && isLocation && lineCount++)
        {
            isLocation = 0;
            checkError(myfile);
            return;
        }
		else
		{
            myfile.close();
            exit_failure("Configfile Error in Line "+ std::to_string(lineCount)+ ": \""+line + "\"");
		}
	}
	if (isLocation)
	{
		myfile.close();
        exit_failure("Configfile Error: missing } in location block");
	}
	checkError(myfile);
}

void		Location::checkError(std::ifstream &myfile)
{
	if(m_root == "" && m_uploadPath.empty() && m_redirection.empty())
    {
        myfile.close();
		exit_failure("Config File Error: Missing root.");
    }
	if(!m_uploadPath.empty() && (!m_root.empty() || !m_index.empty() || !m_method.empty() || m_autoIndex != -1 || !m_redirection.empty()))
	{
        myfile.close();
		exit_failure("Config File Error: Upload Path is specified in location no other directives allowed.");
	}
	if(!m_redirection.empty() && (!m_root.empty() || !m_index.empty() || !m_method.empty() || !m_uploadPath.empty() || m_autoIndex != -1))
	{
        myfile.close();
		exit_failure("Config File Error: redirection is specified in location no other directives allowed.");
	}
	if(!m_uploadPath.empty() && !checkPath(m_uploadPath).empty())
	{
        myfile.close();
		exit_failure("Config File Error: "+ checkPath(m_uploadPath));
	}
	if(!m_uploadPath.empty())
		m_method.push_back("POST");
	if(!m_root.empty() && checkPath(m_root) != "")
	{
        myfile.close();
		exit_failure("Config File Error: "+ checkPath(m_root));
	}
}

void        				Location::setPath(std::string path){m_path = path;}
void        				Location::setRoot(std::string root){m_root = root;}
std::string 				Location::getRedirection(void) const{return(m_redirection);}
std::string     			Location::getPath(void) const{return(m_path);}
int							Location::getAutoIndex(void) const{return(m_autoIndex);}
std::string 				Location::getUploadPath(void) const{return(m_uploadPath);}
std::vector<std::string>	Location::getMethod(void) const{return(m_method);}
bool						Location::getGotMethod(void) const{return(m_gotMethod);}
std::string     			Location::getRoot(void) const{return(m_root);}
std::string     			Location::getIndex(void) const{return(m_index);}
void                        Location::setMethod(std::string method){m_method.push_back(method);}
void						Location::setAutoIndex(int autoIndex){m_autoIndex = autoIndex;}

Location::Location()
{
	m_index 		= "";
	m_root 			= "";
	m_path			= "";
	m_uploadPath 	= "";
	m_autoIndex 	= -1;
	m_redirection = "";
	m_gotMethod = 0;
}
Location::Location(Server server)
{
	m_index = server.getIndex();
	m_root = server.getRoot();
	m_path = "/";
	m_autoIndex = server.getAutoIndex();
	m_method = server.getMethod();
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
	m_gotMethod 	= cp.getGotMethod();
	
	return (*this);
}

Location::~Location(){}
