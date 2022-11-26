#pragma once

#include "../webserv.hpp"

std::string					toString(const int &value);
std::string					toString(const long long &value);
int							max(int a, int b);
int							min(int a, int b);
std::string					getAbsolutePath(std::string path);
std::string					getExtention(std::string path);
std::string					stringToUpper(std::string str);
bool						startsWith(std::string str, std::string start);
FILE        				*createTmpFile(std::string &path);
std::string 				resolvePath(std::string path);
std::string					checkPath(std::string path);
std::vector<std::string>	split(std::string str, std::string sep);
int							remove_directory(const std::string path);