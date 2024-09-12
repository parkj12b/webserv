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

CLIENT_SRC = Client.cpp ContentLine.cpp HeaderLine.cpp StartLine.cpp CgiProcessor.cpp Response.cpp

CLIENT_SRCS = $(addprefix $(CLIENT_SRC_DIR)/, $(CLIENT_SRC))

SERVER_SRC = Server.cpp Kq.cpp

SERVER_SRCS = $(addprefix $(SERVER_SRC_DIR)/, $(SERVER_SRC))

CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

CLIENT_OBJS = $(addprefix $(BIN_DIR)/, $(CLIENT_OBJ))

SERVER_OBJS = $(addprefix $(BIN_DIR)/, $(SERVER_OBJ))

MAIN_SRC = ./main.cpp

MAIN_OBJ = $(BIN_DIR)/main.o

CONFIG_SRC_DIR = ./config/src

CONFIG_SRC = 	Directives.cpp Env.cpp HTTPServer.cpp Lexer.cpp LocationConfig.cpp \
				LocationConfigData.cpp Parser.cpp ServerConfig.cpp \
				ServerConfigData.cpp Syntax.cpp Token.cpp UtilTemplate.cpp \
				Validator.cpp Word.cpp Num.cpp Trie.cpp

CONFIG_SRCS = $(addprefix $(CONFIG_SRC_DIR)/, $(CONFIG_SRC))

CONFIG_OBJ = $(CONFIG_SRC:.cpp=.o)

CONFIG_OBJS = $(addprefix $(BIN_DIR)/, $(CONFIG_OBJ))

INC = -I./config/include -I./server -I./client

BIN_DIR = ./bin

CPP = c++

FLAGS = -std=c++98 -Wall -Wextra -Werror -Ofast -g# -DDEBUG

NAME = webserv


all :
	make -j6 $(NAME)

$(NAME) : $(SERVER_OBJS) $(CLIENT_OBJS) $(MAIN_OBJ) $(CONFIG_OBJS)
	@$(CPP) $(FLAGS) $(SERVER_OBJS) $(CLIENT_OBJS) $(MAIN_OBJ) $(CONFIG_OBJS) $(INC) -o $(NAME)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)
	@echo $(YELLOW)"                    Webserv $(NAME) is ready"$(EOC)
	@echo $(GREEN)"\n==========================================================\n"$(EOC)

$(BIN_DIR) :
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CPP) $(FLAGS) $(INC) -c $< -o $@

$(BIN_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CPP) $(FLAGS) $(INC) -c $< -o $@

$(BIN_DIR)/%.o : $(CONFIG_SRC_DIR)/%.cpp | $(BIN_DIR)
	@$(CPP) $(FLAGS) $(INC) -c $< -o $@

$(BIN_DIR)/main.o : $(MAIN_SRC) | $(BIN_DIR)
	@$(CPP) $(FLAGS) $(INC) -c $(MAIN_SRC) -o $(BIN_DIR)/main.o

clean :
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS) $(MAIN_OBJ) $(CONFIG_OBJS)

fclean : clean
	rm -rf $(NAME)

re :
	@make fclean
	@make all

.PHONY : clean fclean re all