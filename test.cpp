
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>

extern char** environ;


int main()
{
	pid_t pid;
	int ret = 0;
	int fd = open("/tmp/hello", O_RDWR | O_CREAT, 0777);
    
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", "", 1);
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("SCRIPT_FILENAME", "/Users/ayoubtabout/Desktop/Webo-Servo/static_html/test.php", 1); // Parse file on request level
	setenv("SERVER_SOFTWARE", "TaboutZmer", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);

	pid = fork();

	int status = 0;
	if (pid == 0)
	{
		dup2(fd, 1);
		char *args[3];
		args[0] = "/opt/homebrew/bin/php-cgi";
		args[1] = "/Users/ayoubtabout/Desktop/Webo-Servo/static_html/test.php";
		args[2] = NULL;
		if (!(execve(args[0], args, environ)))
            std::cout << "shit failed" << std::endl;
	}
	else
	{
		while (waitpid(-1, &status, WUNTRACED) > 0)
			;
	}
	char buffer[1024] = {0};
	lseek(fd, 0, SEEK_SET);
	std::string res;
	while (read(fd, buffer, 1024) > 0)
        res += buffer;
	close(fd);
	std::cout << "Received shit: " << res << std::endl;
}