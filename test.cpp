#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <map>
#include <string.h>
#include <stdio.h>

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

int min(int a, int b)
{
	return (a < b ? a : b);
}

#define BUFFER_SIZE 11
void	peekBody(char *buf, long *chunksize, long m_bodySize, long *m_bodyCursor, FILE *m_bodyFile, bool *done)
{
	// const int restsize = m_bodySize - *m_bodyCursor + 1;
	// *chunksize = min(restsize , BUFFER_SIZE);
	// if (*chunksize  BUFFER_SIZE)
	// 	std::cout << *chunksize << std::endl;
	// fseek(m_bodyFile, *m_bodyCursor , SEEK_SET);
	// std::cout << "ftell: " << ftell(m_bodyFile) << std::endl;
	// std::cout << "m_bodyCursor: " << *m_bodyCursor << std::endl;
	fgets(buf, *chunksize, m_bodyFile);
	// if (feof(m_bodyFile))
	// 	*done = true;
	if (restsize < BUFFER_SIZE)
		*done = true;
	// if (*m_bodyCursor == m_bodySize)
	// {
	// 	*done = true;
	// 	return ;
	// }
	*m_bodyCursor += *chunksize - 1;
}

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "usage: ./a.out [path]" << std::endl;
		return 1;
	}
	std::string path = argv[1];
	FILE *m_bodyFile = fopen(path.c_str(), "r");
	long m_bodySize = 0;
	long m_bodyCursor = 0;
	char buf[BUFFER_SIZE];
	long chunksize = 0;
	// get body size
	fseek(m_bodyFile, 0L, SEEK_END);
	m_bodySize = ftell(m_bodyFile);
	rewind(m_bodyFile);
	// peek body loop
	bool done = false;
	while (!done)
	{
		peekBody(buf, &chunksize, m_bodySize, &m_bodyCursor, m_bodyFile, &done);
		// std::cout << buf;
		// std::cout << "cursor: " << m_bodyCursor << std::endl;
		// std::cout << "chunksize: " << chunksize << std::endl;
		// std::cout << "buf: " << buf << std::endl;
	}
}