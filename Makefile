NAME = webserv
SRCS = TcpListener/TcpListener.cpp \
		TcpListener/Sockets.cpp \
		configFileParser/Location.cpp \
		configFileParser/Server.cpp \
		configFileParser/configParser.cpp \
		Request/Request.cpp \
		main.cpp \

FLAGS = -Wall -Wextra -Werror  -std=c++98

all: $(NAME)

$(NAME): $(SRCS)
	c++ -g $(FLAGS) $(SRCS) -o $(NAME)
clean:
	rm -rf $(NAME)
fclean: clean
	rm -rf $(NAME)
re: fclean all