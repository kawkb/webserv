#include "Location.hpp"

Location::Location()
{
    //std::cout << "We've got a location" << std::endl;
}

void Location::debug()
{
    std::cout << std::endl << "Location: " << _path << " config" << std::endl;

    std::unordered_map<std::string, std::string>::iterator it = keys.begin();

    while(it != keys.end())
    {
        std::cout << it->first << " :: " << it->second << std::endl;
        it++;
    }
}

void Location::setPath(std::string path)
{
    _path = path; 
}

std::string Location::getPath()
{
    return _path;
}

std::vector<std::string> split(std::string string, char delimiter)
{
    std::size_t found = string.find(delimiter);
    std::vector<std::string> pairs;
    if (found != std::string::npos)
    {
        pairs.push_back(string.substr(0, found));
        pairs.push_back(string.substr(found + 1, string.length() - found - 2));
    }
    return pairs;
}

bool Location::attach(std::string pair)
{
    std::vector<std::string> pairs = split(pair, ':');
    if (!pairs.empty())
        keys[pairs[0]] = pairs[1];
    else
       std::cout << "This shit so empty" << std::endl;
    return 1;
}

std::string Location::getKey(std::string key)
{
    return (keys[key]);
}
