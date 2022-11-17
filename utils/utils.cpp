#include "utils.hpp"

template <class T>
std::string toString(const T &value)
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

// convert relative path to absolute path
std::string getAbsolutePath(std::string path)
{
	char *absolutePath = realpath(path.c_str(), NULL);
	if (absolutePath == NULL)
		return "";
	std::string result(absolutePath);
	free(absolutePath);
	return result;
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
