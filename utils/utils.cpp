/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moerradi <moerradi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 20:48:34 by kdrissi-          #+#    #+#             */
/*   Updated: 2022/11/23 02:18:16 by moerradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string toString(const int &value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string toString(const long long &value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int max(int a, int b)
{
	return (a > b ? a : b);
}

int min(int a, int b)
{
	return (a < b ? a : b);
}

std::string getAbsolutePath(std::string path)
{
	std::vector<std::string> v;
	size_t n = path.length();
	std::string ans;
	for (size_t i = 0; i < n; i++)
	{
		std::string dir = "";
		while (i < n && path[i] != '/')
		{
			dir += path[i];
			i++;
		}
		if (dir == "..")
		{
			if (!v.empty())
				v.pop_back();
		}
		else if (dir != "." && dir != "")
		{
			v.push_back(dir);
		}
	}
	std::vector<std::string>::iterator i;
	for (i = v.begin(); i != v.end(); i++)
	{
		ans += "/" + *i;
	}
	if (path[path.length() - 1] == '.')
		ans += "/";
	if (ans == "")
		return "/";

	return path[path.length() - 1] == '/' ? ans + "/" : ans;
}

std::string stringToUpper(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}

std::string getExtention(std::string path)
{
	size_t pos = path.find_last_of(".");
	if (pos == std::string::npos)
		return "";
	return path.substr(pos);
}

bool startsWith(std::string str, std::string start)
{
	if (str.length() < start.length())
		return false;
	for (size_t i = 0; i < start.length(); i++)
	{
		if (str[i] != start[i])
			return false;
	}
	return true;
}

FILE *createTmpFile(std::string &path)
{
    std::string tmpdir = P_tmpdir;
    // generat unique string
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(7);

    for (int i = 0; i < 7; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    std::string tmpfile = tmpdir + "/webservir" + tmp_s;
    // check if file already exists
    struct stat buffer;
    if (stat(tmpfile.c_str(), &buffer) == 0)
        return createTmpFile(path);
    // create file
    FILE *fs = fopen(tmpfile.c_str(), "w");
    if (fs == NULL)
        return NULL;
    path = tmpfile;
    return fs;
}

std::vector<std::string> split(std::string str, std::string sep)
{
	std::vector<std::string> ret;
	std::string token;
	size_t pos = str.find(sep);
	if (pos == std::string::npos)
	{
		ret.push_back(str);
		return ret;
	}
	while ((pos = str.find(sep)) != std::string::npos)
	{
		token = str.substr(0, pos);
		ret.push_back(token);
		str.erase(0, pos + sep.length());
	}
	ret.push_back(str);
	return ret;
}