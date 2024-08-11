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

CLIENT_SRC_DIR = ./client

SERVER_SRC_DIR = ./server

CLIENT_SRC = Client.cpp ContentLine.cpp HeaderLine.cpp StartLine.cpp

CLIENT_SRCS = $(addprefix $(CLIENT_SRC_DIR)/, $(CLIENT_SRC))

SERVER_SRC = Server.cpp Kq.cpp Response.cpp

SERVER_SRCS = $(addprefix $(SERVER_SRC_DIR)/, $(SERVER_SRC))

CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

CLIENT_OBJS = $(addprefix $(BIN_DIR)/, $(CLIENT_OBJ))

SERVER_OBJS = $(addprefix $(BIN_DIR)/, $(SERVER_OBJ))

BIN_DIR = ./bin

CPP = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -c
name = webserv

MAIN_SRC = ./main.cpp

MAIN_OBJ = $(BIN_DIR)/main.o

all : $(name)

$(name) : $(SERVER_OBJS) $(CLIENT_OBJS)
	$(CPP) $(SERVER_OBJS) $(CLIENT_OBJS) -I./server -I./client ./main.o -o $(name)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)
	@echo $(YELLOW)"                    Webserv $(name) is ready"$(EOC)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)

$(BIN_DIR) :
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CPP) $(FLAGS) $< -o $@

$(BIN_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CPP) $(FLAGS) $< -o $@

$(BIN_DIR)/%.o: $(MAIN_SRC) | $(BIN_DIR)
	@$(CPP) $(FLAGS) $< -o $@

clean :
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean :
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)
	rm -f $(name)

re :
	@make fclean
	@make all

.PHONY : clean fclean re all