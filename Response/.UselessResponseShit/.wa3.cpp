void	setenvCGI(Request request)
{
	if (request.getMethod() == "GET")
	{
		std::string query = keys["query"];	
		if (query.size() != 0)
		{
			std::string c_size = std::to_string(keys["query"].length());
			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
		}		
		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	}
	else if (request.getMethod() == "POST")
	{
		std::string content_type = request.getHeader("Content-Type");
		if (!(content_type.empty()))
			setenv("CONTENT_TYPE", content_type.c_str(), 1);
		else
			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
		std::string content_length = request.getHeader("Content-Length");
		if (!(content_length.empty()))
			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
	}
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", keys["query"].c_str(), 1);
	setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
	setenv("SCRIPT_FILENAME", keys["full_file_path"].c_str(), 1); // Parse file on request level
	setenv("SERVER_SOFTWARE", "Test", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "true", 1);
}

FILE    *Response::serveCgi(Request request)
{
	setenvCgi(request);
	// create temporary file for cgi response using tempfile
	FILE *tempfile = tmpfile();
	if (tempfile == NULL)
	{
		std::cout << "Error creating temporary file" << std::endl;
		return ;
	}
	// dup standard output to temporary file
	int stdout_copy = dup(1);
	dup2(fileno(tempfile), 1);
	// formulate environement variables
	// if get request, add query string to environement variables
	// if post request, add content type and content length to environement variables
	// if post request, dup standard input to request body file descriptor
	pid_t pid;
	pid = fork();
	int status = 0;
	if (pid == 0)
	{
		if (request.getMethod() == "POST")
		{
			int stdin_copy = dup(0);
			// write request body to temporary file
			FILE *body = tmpfile();
			if (body == NULL)
			{
				std::cout << "Error creating temporary file" << std::endl;
				return ;
			}
			std::string body_str = request.keys["body"];
			fwrite(body_str.c_str(), 1, body_str.length(), body);
			rewind(body);
			// dup standard input to temporary file
			dup2(fileno(body), 0);
			// execute cgi script
		}
		if (keys["extension"] == ".php")
		{
			char *args[3];
			args[0] = (char *)keys["cgi_path"].c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
		else if (keys["extension"] == ".py")
		{
			char *args[3];
			std::string python = "/usr/bin/python";
			args[0] = (char *)python.c_str();
			args[1] = (char *)keys["full_file_path"].c_str();
			args[2] = NULL;
			execve(args[0], args, environ);
		}
 // environ is a variable declared in unistd.h, and it keeps track of the environment variables during this running process.
	}
	else
	{
		time_t t = time(NULL);
		while (time(NULL) - t < 5)
		{
			if (waitpid(pid, &status, WNOHANG) != 0)
				break;
		}
	}
	// restore standard output
	dup2(stdout_copy, 1);
	close(stdout_copy);
	// read temporary file into response body
	rewind(tempfile);
	return (tempfile);
	// char buffer[1024];
	// std::string res;
	// while (fgets(buffer, 1024, tempfile))
	// {
	// 	res += buffer;
	// }
	// printf("%s", keys["body"].c_str());
	// close temporary file
	// fclose(tempfile);
	// keys["version"] = "HTTP/1.1";
	// // set content length
	// keys["code"] = "200";
	// keys["phrase"] = "OK";
	// keys["body"] = res;
	// // dprintf(2,"%s\n", res.c_str());
    // appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
    // appendHeader("Content-Type: text/html");
}

// std::string    Response::serveCgi(Request request)
// {
//     //std::string reqtype = _req.getMethod();
// 	pid_t pid;
// 	int ret = 0;
// 	int fd = open("/tmp/hello", O_RDWR | O_CREAT, 0777);
// 	int fbody = open(, O_RDWR | O_CREAT, 0777);
// 	// There will always be a reqtype; so no need to check here. But a check might be done getKey level either throw an exception ot check if empty()
// 	if (request.getMethod() == "GET")
// 	{
// 		std::string query = keys["query"];	
// 		if (query.size() != 0)
// 		{
// 			std::string c_size = std::to_string(keys["query"].length());
// 			setenv("CONTENT_LENGTH", c_size.c_str(), 1);
// 		}		
// 		setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
// 	}
// 	else if (request.getMethod() == "POST")
// 	{
// 		std::string content_type = request.getHeader("Content-Type");
// 		if (!(content_type.empty()))
// 			setenv("CONTENT_TYPE", content_type.c_str(), 1);
// 		else
// 			setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
// 		std::string content_length = request.getHeader("Content_length");
// 		if (!(content_length.empty()))
// 			setenv("CONTENT_LENGTH", content_length.c_str(), 1);
// 	}
    
// 	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
// 	setenv("QUERY_STRING", keys["query"].c_str(), 1);
// 	// std::cout << "WAA TFA7 <<>><><>" << keys["query"].c_str() << std::endl;
// 	setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
// 	setenv("SCRIPT_FILENAME", keys["full_file_path"].c_str(), 1); // Parse file on request level
// 	setenv("SERVER_SOFTWARE", "Test", 1);
// 	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
// 	setenv("REDIRECT_STATUS", "true", 1);
// 	pid = fork();
// 	int status = 0;
// 	if (pid == 0)
// 	{
// 		if (request.getMethod() == "POST")
// 			dup2(fbody, 0);
// 		else
// 			dup2(fd, 0);
// 		dup2(fd, 1);
// 		if (keys["extension"] == ".php")
// 		{
// 			char *args[3];
// 			args[0] = (char *)keys["cgi_path"].c_str();
// 			args[1] = (char *)keys["full_file_path"].c_str();
// 			args[2] = NULL;
// 			execve(args[0], args, environ);
// 		}
// 		else if (keys["extension"] == ".py")
// 		{
// 			char *args[3];
// 			std::string python = "/usr/bin/python";
// 			args[0] = (char *)python.c_str();
// 			args[1] = (char *)keys["full_file_path"].c_str();
// 			args[2] = NULL;
// 			execve(args[0], args, environ);
// 		}
//  // environ is a variable declared in unistd.h, and it keeps track of the environment variables during this running process.
// 	}
// 	else
// 	{
// 		time_t t = time(NULL);
// 		while (time(NULL) - t < 5)
// 		{
// 			if (waitpid(pid, &status, WNOHANG) != 0)
// 				break;
// 		}
// 	}
// 	char buffer[1024] = {0};
// 	lseek(fd, 0, SEEK_SET);
// 	std::string res;
// 	while (read(fd, buffer, 1024) > 0)
//         res += buffer;
// 	close(fd);
// 	close(fbody);
// 	return res;
// 	// keys["version"] = "HTTP/1.1";
//     // keys["code"] = "200";
//     // keys["phrase"] = "OK";

//     // keys["body"] = res;

// 	// std::remove("/tmp/hello");
// 	// std::remove("/cgi/body");
//     // appendHeader("Content-Length: " + std::to_string(keys["body"].length()));
//     // appendHeader("Content-Type: text/html");
// 	// std::cout << "Received shit: " << res << std::endl;
// }
