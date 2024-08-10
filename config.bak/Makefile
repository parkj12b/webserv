
SRC =	Directives.cpp Env.cpp HTTPServer.cpp Lexer.cpp LocationConfig.cpp \
		LocationConfigData.cpp main.cpp Parser.cpp ServerConfig.cpp \
		ServerConfigData.cpp Syntax.cpp Token.cpp UtilTemplate.cpp \
		Validator.cpp Word.cpp Num.cpp

SRCS = $(addprefix src/, $(SRC))

CPPFLAGS = -std=c++11 -Wall -Wextra -Werror -g

OBJS = $(SRCS:.cpp=.o)

CPP = c++

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -Iinclude -c $< -o $@

all:
	c++ -std=c++98 -Wall -Wextra -Werror -g main.cpp UtilTemplate.cpp

high: $(OBJS)
	c++ $(CPPFLAGS) -I./include $(OBJS)

clean:
	rm -f $(OBJS)

.PHONY: all clean