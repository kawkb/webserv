NAME = webserv
SRCS = TcpListener/TcpListener.cpp \
		TcpListener/Sockets.cpp \
		TcpListener/Webserv.cpp \
		configFileParser/Location.cpp \
		configFileParser/Server.cpp \
		configFileParser/configParser.cpp \
		Request/Request.cpp \
		main.cpp \
		Response/Response.cpp \
		Response/cgi.cpp \
		utils/utils.cpp

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror  -std=c++98 -fsanitize=address  -g3

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)
re: fclean all