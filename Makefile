NAME = webserv 
CC = g++
FLAGS = -Wall -Wextra -Werror --std=c++98 -g

SRCS = main.cpp parser.cpp Exceptions.cpp request.cpp Server.cpp
RM =  rm -rf

.cpp.o:
	@c++ ${FLAGS}  -c $< -o ${<:.cpp=.o}

OBJS = ${SRCS:.cpp=.o}

all: ${NAME}

${NAME}:${OBJS} 
		@${CC} ${OBJS} ${FLAGS} -o ${NAME}

run:  all 
	./${NAME} 
	

val: all
	valgrind  --leak-check=yes --track-origins=yes -s --trace-children=yes  ./${NAME}

clean:
	    ${RM} ${OBJS}



test: all 
	./${NAME} & sleep  1 && curl http://localhost:9995/api
	@echo "Testing done."

fclean: clean
	@${RM} ${NAME}
	@${RM} ${NAME_TEST}

git:
		@git add ${SRCS} Makefile

re: fclean all
	
.PHONY: clean fclean re all run git

