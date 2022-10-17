NAME= webserv
SRC_DIR := ./src
FLAGS = -Wall -Werror -Wextra   --std=c++98 -g 
OBJ_DIR := ./obj
SRC_FILES := ./src/Exceptions.cpp ./src/main.cpp ./src/parser.cpp ./src/Server.cpp ./src/server_utils.cpp ./src/response.cpp ./src/CGI.cpp ./src/upload.cpp
Header := ./src/Server.hpp ./src/Exceptions.hpp ./src/parser.hpp  ./src/response.hpp ./src/CGI.hpp ./src/upload.hpp ./src/config_structs.hpp
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
all : $(OBJ_DIR) $(NAME)
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
$(NAME): $(OBJ_FILES) 
		g++ $(LDFLAGS) -o $@ $^
run : $(NAME)
		./$(NAME)
re: fclean all

val: all
	valgrind  --leak-check=yes --track-origins=yes -s --trace-children=yes  ./${NAME}
clean:
		rm -rf $(OBJ_DIR)
git:
		git add $(SRC_FILES) $(Header) Makefile 
fclean: clean
		rm -f webserv

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
		g++  $(FLAGS) -c -o $@ $<
.PHONY: clean fclean re all run git