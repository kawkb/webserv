/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDelete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 12:12:04 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/11/16 16:58:39 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


int remove_directory(const std::string path)
{
	DIR *d = opendir(path.c_str());
	int r = -1;
	if (d)
	{
		struct dirent *p;
		r = 0;
		while (!r && (p = readdir(d)))
		{
			int r2 = -1;
			std::string filename(p->d_name);
			if (filename == "." || filename == "..")
				continue;
			const std::string filepath = path + "/" + filename;
			struct stat statbuf;
			if (!stat(filepath.c_str(), &statbuf))
			{
				if (S_ISDIR(statbuf.st_mode))
					r2 = remove_directory(filepath);
				else
					r2 = unlink(filepath.c_str());
			}
			r = r2;
		}
		closedir(d);
	}
	if (!r)
	  r = rmdir(path.c_str());
	return r;
}

bool Response::handleDelete()
{
	// location path should never end with a slash except if it is literally /
	// root path should always end with a slash
	Location location = m_request.getLocation();
	Server server = m_request.getServer();
	std::string path = location.getPath();
	std::string root = location.getRoot();
	std::string uri = m_request.getUri();
	// parse request path
	m_filePath = root + uri.substr(path.size());
	// resolve path
	std::string absolute = getAbsolutePath(m_filePath);
	if (absolute == "")
	{
		m_statusCode = "404";
		return false;
	}
	if (absolute.find(root) != 0)
	{
		m_statusCode = "403";
		return false;
	}
	if (absolute != m_filePath)
	{
		m_statusCode = "301";
		m_headersMap["Location"] = path + absolute.substr(root.size());
		return true;
	}
	struct stat fileStat;
	if (stat(m_filePath.c_str(), &fileStat) != 0)
	{
		if (errno == EACCES)
			m_statusCode = "403";
		else
		
			m_statusCode = "404";
		return false;
	}
	if (S_ISDIR(fileStat.st_mode))
	{
		if (m_filePath[m_filePath.size() - 1] != '/')
		{
			m_statusCode = "301";
			m_headersMap["Location"] = uri + "/";
			return false;
		}
		if (remove_directory(m_filePath) != 0)
		{
			if (errno == EACCES)
				m_statusCode = "403";
			else if (errno == ENOENT)
				m_statusCode = "404";
			else
				m_statusCode = "500";			
			return false;
		}
	}
	else if (S_ISREG(fileStat.st_mode))
	{
		if (unlink(m_filePath.c_str()) != 0)
		{
			if (errno == EACCES)
				m_statusCode = "403";
			else if (errno == ENOENT)
				m_statusCode = "404";
			else
				m_statusCode = "500";
			return false;
		}
	}
	else
	{
		m_statusCode = "403";
		return false;
	}
	m_statusCode = "204";
	return true;
}