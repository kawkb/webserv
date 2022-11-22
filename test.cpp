#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

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

int main(int ac, char **av)
{
	int fd = open(av[1], O_RDONLY);
	char buf[1024];
	int r;
	std::string ret;
	while ((r = read(fd, buf, 1024)) > 0)
	{
		ret += std::string(buf, r);
	}
	// std::cout << ret << std::endl;
}