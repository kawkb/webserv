#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>

// int main()
// {
// 	FILE *fp = fopen("foo", "r");
// 	char *line = NULL;
// 	size_t len = 0;
// 	ssize_t read;
// 	std::map<std::string, std::string> m_headersMap;
// 	while ((read = getline(&line, &len, fp) != -1)) {
// 				if (strcmp(line, "\r\n") == 0)
// 			break;
// 		std::string header = line;
// 		std::string key = header.substr(0, header.find(":"));
// 		std::string value = header.substr(header.find(":") + 2);
// 		// remove last 2 chars from value
// 		if (value.size() > 2)
// 			value = value.substr(0, value.size() - 2);
// 		// remove trailing newline from value	
// 		// value.erase(value.find_last_not_of(" \n\r\t")+1);
// 		m_headersMap[key] = value;
// 		free(line);
// 		line = NULL;
// 	}
// 	// print header values
// 	for (std::map<std::string, std::string>::iterator it = m_headersMap.begin(); it != m_headersMap.end(); ++it)
// 		std::cout << it->first << " =>" << it->second << std::endl;
// 	// copy what's left to a tmpfile
// 	// FILE *tmp = tmpfile();
// 	// char buf[1024];
// 	// while ((read = fread(buf, 1, 1024, fp)) > 0) {
// 	// 	std::cout << read << std::endl;
// 	// 	fwrite(buf, 1, read, tmp);
// 	// }
// 	// fclose(fp);
// 	// rewind(tmp);
// 	// // print file contents
// 	// while ((read = getline(&line, &len, tmp)) != -1) {
// 	// 	std::cout << line;
// 	// }
// }

// std::string getAbsolutePath(std::string path)
// {
// 	std::vector<std::string> v;
// 	size_t n = path.length();
// 	std::string ans;
// 	for (int i = 0; i < n; i++)
// 	{
// 		std::string dir = "";
// 		while (i < n && path[i] != '/')
// 		{
// 			dir += path[i];
// 			i++;
// 		}
// 		if (dir == "..")
// 		{
// 			if (!v.empty())
// 				v.pop_back();
// 		}
// 		else if (dir != "." && dir != "")
// 		{
// 			v.push_back(dir);
// 		}
// 	}
// 	std::vector<std::string>::iterator i;
// 	for (i = v.begin(); i != v.end(); i++)
// 	{
// 		ans += "/" + *i;
// 	}
// 	if (ans == "")
// 		return "/";

// 	return ans;
// }

// bool startsWith(std::string str, std::string start)
// {
// 	if (str.length() < start.length())
// 		return false;
// 	for (size_t i = 0; i < start.length(); i++)
// 	{
// 		if (str[i] != start[i])
// 			return false;
// 	}
// 	return true;
// }

// int main(int ac, char **av)
// {
// 	if (ac != 3)
// 		return 1;
// 	std::cout << startsWith(std::string(av[1]) + "/", av[2])<< std::endl;
// }



int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	std::string path = av[1];
	// std::string extenstion = path.substr(path.find_last_of(".") + 1);
	size_t pos = path.find_last_of(".");
	std::string extenstion = "";
	if (pos != std::string::npos)
		extenstion = path.substr(pos + 1);
	
	std::cout << extenstion << std::endl;
}