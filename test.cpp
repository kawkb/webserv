// #include <string>
// #include <fstream>
// #include <iostream>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <dirent.h>
// #include <vector>
// #include <map>
// #include <string.h>
// #include <stdio.h>
// #include<cstdlib>




// int min(int a, int b)
// {
// 	return (a < b ? a : b);
// }

// #define BUFFER_SIZE 12
// void	peekBody(char *buf, long *chunksize, long m_bodySize, long *m_bodyCursor, FILE *m_bodyFile, bool *done)
// {
// 	*chunksize = min(BUFFER_SIZE, m_bodySize - *m_bodyCursor);
// 	if (*chunksize == 0)
// 	{
// 		buf[*chunksize] = '\0';
// 		*done = true;
// 		return ;
// 	}
// 	fseek(m_bodyFile, *m_bodyCursor, SEEK_SET);
// 	fread(buf, *chunksize,1, m_bodyFile);
// }

// // generate random int between to values
// int irand(int min, int max) {
//     return ((double)rand() / ((double)RAND_MAX + 1.0)) * (max - min + 1) + min;
// }

// int main (int argc, char **argv)
// {
// 	if (argc != 2)
// 	{
// 		std::cout << "usage: ./a.out [path]" << std::endl;
// 		return 1;
// 	}
// 	std::string path = argv[1];
// 	FILE *m_bodyFile = fopen(path.c_str(), "r");
// 	long m_bodySize = 0;
// 	long m_bodyCursor = 0;
// 	char buf[BUFFER_SIZE];
// 	long chunksize = BUFFER_SIZE;
// 	// get body size
// 	fseek(m_bodyFile, 0L, SEEK_END);
// 	m_bodySize = ftell(m_bodyFile);
// 	rewind(m_bodyFile);
// 	// peek body loop
// 	bool done = false;
// 	while (!done)
// 	{
// 		peekBody(buf, &chunksize, m_bodySize, &m_bodyCursor, m_bodyFile, &done);
// 		// std::cout << buf;
// 		// read a random size
// 		// virtual send-----
// 		int randsize = irand(1, chunksize);
// 		if (randsize + m_bodyCursor > m_bodySize)
// 			randsize = m_bodySize - m_bodyCursor;
// 		char *tmp = (char *)malloc(randsize + 1);
// 		memcpy(tmp, buf , randsize);
// 		tmp[randsize] = '\0';
// 		std::cout << tmp;
// 		// ----
// 		m_bodyCursor += randsize;
// 	}
// }


#include <string>
#include <iostream>
#include <cstdlib>

std::string getAbsolutePath(std::string path)
{
	char *absolutePath = realpath(path.c_str(), NULL);
	if (absolutePath == NULL)
		return "";
	std::string result(absolutePath);
	std::free(absolutePath);
	return result;
}


std::string stringToUpper(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}
int main(int argc, char **argv)
{
	std::cout << stringToUpper(argv[1]);
}