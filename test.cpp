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

extern char **environ;

int main(int ac, char **av)
{
	setenv("REDIRECT_STATUS", "200", true);
	setenv("GETAWAY_INTERFACE","CGI/1.1", true);
    setenv("SERVER_PROTOCOL","HTTP/1.1", true);
    setenv("FCGI_ROLE","RESPONDER", true);
    setenv("REQUEST_SCHEME","http", true);
    setenv("SERVER_SOFTWARE","webserv/1.1", true);
    setenv("REMOTE_ADDR","0.0.0.0", true);
    setenv("REMOTE_PORT","0", true);
    setenv("SCRIPT_FILENAME", "/Users/moerradi/webserv/serve/bab.php", true);

	const char *argv[] = {"/Users/moerradi/.brew/bin/php-cgi", "/Users/moerradi/webserv/serve/bab.php", NULL};
	execve(argv[0], (char *const *)argv, environ);
}