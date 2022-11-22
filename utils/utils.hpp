#pragma once

#include "../webserv.hpp"

template <class T>
std::string	toString(const T &value);
int			max(int a, int b);
int			min(int a, int b);
std::string	getAbsolutePath(std::string path);
std::string	getExtention(std::string path);
std::string	stringToUpper(std::string str);
bool		startsWith(std::string str, std::string start);
FILE        *createTmpFile(std::string &path);