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
#include<cstdlib>
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

#define BUFFER_SIZE 12
void	peekBody(char *buf, long *chunksize, long m_bodySize, long *m_bodyCursor, FILE *m_bodyFile, bool *done)
{
	*chunksize = min(BUFFER_SIZE, m_bodySize - *m_bodyCursor);
	if (*chunksize == 0)
	{
		buf[*chunksize] = '\0';
		*done = true;
		return ;
	}
	fseek(m_bodyFile, *m_bodyCursor, SEEK_SET);
	fread(buf, *chunksize,1, m_bodyFile);
}

// generate random int between to values
int irand(int min, int max) {
    return ((double)rand() / ((double)RAND_MAX + 1.0)) * (max - min + 1) + min;
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
	long chunksize = BUFFER_SIZE;
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
		// read a random size
		// virtual send-----
		int randsize = irand(1, chunksize);
		if (randsize + m_bodyCursor > m_bodySize)
			randsize = m_bodySize - m_bodyCursor;
		char *tmp = (char *)malloc(randsize + 1);
		memcpy(tmp, buf , randsize);
		tmp[randsize] = '\0';
		std::cout << tmp;
		// ----
		m_bodyCursor += randsize;
	}
}