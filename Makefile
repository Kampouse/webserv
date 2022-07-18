NAME = webserv 
NAME_TEST = container_test
FLAGS = -Wall -Wextra -Werror --std=c++11 -g -DHEADER=Ft
FLAGS_test = -Wall -Wextra -Werror --std=c++98 -g -DHEADER=std

SRCS = main.cpp parser.cpp 
RM =  rm -rf 
.cpp.o:
	@c++ ${FLAGS}  -c $< -o ${<:.cpp=.o}

.cpp.o.test:
	@c++ ${FLAGS_test}  -c $< -o ${<:.cpp=.o.test}
OBJS = ${SRCS:.cpp=.o}
OBJS_test = ${SRCS:.cpp=.o}

CC = g++

all: ${NAME} 
${NAME}:${OBJS} 
		@${CC} ${OBJS}   ${FLAGS}   -o ${NAME}
test: ${NAME_TEST}
${NAME_TEST}:${OBJS_test}
		@${CC} ${OBJS_test}   ${FLAGS_test}   -o ${NAME_TEST}
run:  all 
	./${NAME}
val: all
	valgrind  --leak-check=yes --track-origins=yes -s --trace-children=yes  ./${NAME}

clean:
	    ${RM} ${OBJS}	
git:
		@git add ${SRCS} Makefile 

fclean: clean
	@${RM} ${NAME}
	@${RM} ${NAME_TEST}
re: fclean all
	
.PHONY: clean fclean re all run git

