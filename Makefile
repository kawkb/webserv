NAME = webserv
SRCS = TcpListener/TcpListener.cpp \
		TcpListener/Webserv.cpp \
		configFileParser/Location.cpp \
		configFileParser/Server.cpp \
		configFileParser/configParser.cpp \
		Request/Request.cpp \
		main.cpp \
		Response/Response.cpp \
		Response/cgi.cpp \
		utils/utils.cpp

INCLUDES= TcpListener/TcpListener.hpp \
		TcpListener/Webserv.hpp \
		configFileParser/Location.hpp \
		configFileParser/Server.hpp \
		Request/Request.hpp \
		Response/Response.hpp \
		utils/utils.hpp

OBJS = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror  -std=c++98

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	c++ $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)
re: fclean all