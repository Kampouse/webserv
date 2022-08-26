NAME = webserv 
CC = g++
FLAGS =   --std=c++98 -g

SRCS = Exceptions.cpp main.cpp parser.cpp utils.cpp Server.cpp server_utils.cpp  response.cpp CGI.cpp


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
	./${NAME} & sleep  1 && http :9991  

fclean: clean
	@${RM} ${NAME}
	@${RM} ${NAME_TEST}

git:
		@git add ${SRCS} Makefile Exceptions.hpp config_structs.hpp parser.hpp utils.hpp Server.hpp response.hpp CGI.hpp

re: fclean all
	
.PHONY: clean fclean re all run git

