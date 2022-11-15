#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <map>

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
				{
					r2 = 0;
					std::cout << filepath << std::endl;
					// r2 = unlink(buf);
				}
			}
			r = r2;
		}
		closedir(d);
	}

	// if (!r)
	// 	std::cout << path << std::endl;
	//   r = rmdir(path);

	return r;
}

std::vector<std::string> split(std::string str, std::string sep)
{

	
}

void	fillQueryString(std::string &m_uri, std::map<std::string, std::string> &m_queryString)
{

}

int main (int argc, char **argv)
{
	std::string path = argv[1];
	std::map<std::string, std::string> m_queryString;
	fillQueryString(path, m_queryString);
	// print query string
	std::cout <<"path : " + path << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = m_queryString.begin(); it != m_queryString.end(); it++)
	{
		std::cout << "Key : " << it->first << " --- Value : " << it->second << std::endl;
	}
	return 0;
}