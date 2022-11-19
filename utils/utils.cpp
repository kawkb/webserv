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

std::string getAbsolutePath(std::string path)
{
	std::vector<std::string> v;
	size_t n = path.length();
	std::string ans;
	for (int i = 0; i < n; i++)
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
	if (ans == "")
		return "/";

	return ans;
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