#-------------------------------------------

BLACK       =   "\033[0;30m"
GRAY        =   "\033[1;30m"
RED         =   "\033[1;31m"
GREEN       =   "\033[0;32m"
YELLOW      =   "\033[1;33m"
PURPLE      =   "\033[0;35m"
CYAN        =   "\033[1;36m"
WHITE       =   "\033[1;37m"
EOC         =   "\033[0;0m"
LINE_CLEAR  =   "\x1b[1A\x1b[M"

#-------------------------------------------

SOURCE = ./client/Client.cpp \
		./client/EntityLine.cpp \
		./client/HeaderLine.cpp \
		./client/StartLine.cpp \
		./server/Server.cpp \
		./main.cpp
OBJS = $(SOURCE:.cpp=.o)
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -c
name = webserv

all : $(name)

$(name) : $(OBJS)
	@$(CC) $(OBJS) -o $(name)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)
	@echo $(YELLOW)"                    Webserv $(name) is ready"$(EOC)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)

%.o : %.cpp
	@$(CC) $(FLAGS) $< -o $@

clean :
	rm -f $(OBJS)

fclean :
	rm -f $(OBJS)
	rm -f $(name)

re :
	@make fclean
	@make all

.PHONY : clean fclean re all