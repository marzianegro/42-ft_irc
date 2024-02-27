# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mnegro <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/24 18:25:00 by mnegro            #+#    #+#              #
#    Updated: 2024/02/27 18:18:52 by mnegro           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### PHONY TARGET ###
.PHONY: all clean fclean re

### SILENT TARGET ###
.SILENT:

### VARIABLES (DEFINITION) ###
NAME_C = client
NAME_S = server

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
RM = rm -f

#### TARGETS ####
SRCS = client.cpp server.cpp

OBJS = ${SRCS:.cpp=.o}

### (EXPLICIT) RULES ###
%.o: %.cpp
	@echo "${YELLOW}Compiling:${WHITE} $< ${DEF_COLOR}"
	${CC} ${CFLAGS} -c $< -o $@

all:	${NAME_C} ${NAME_S}

${NAME_C}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME_C} client.o
	@echo "Rule '${GREEN}all${DEF_COLOR}' for ${NAME_C} executed successfully!"

${NAME_S}: ${OBJS}
	${CC} ${CFLAGS} -o ${NAME_S} server.o
	@echo "Rule '${GREEN}all${DEF_COLOR}' for ${NAME_S} executed successfully!"


clean:
	${RM} ${OBJS}
	@echo "Rule '${GREEN}clean${DEF_COLOR}' for ${NAME_C} & ${NAME_S} executed successfully!"

fclean:
	${RM} ${OBJS} ${NAME_C} ${NAME_S}
	@echo "Rule '${GREEN}fclean${DEF_COLOR}' for ${NAME_C} & ${NAME_S} executed successfully!"

re:	fclean all
	@echo "Rule '${GREEN}re${DEF_COLOR}' for ${NAME_C} & ${NAME_S} executed successfully!"

### (BRIGHT) COLORS ###
DEF_COLOR = \033[0;39m
BLACK = \033[1;90m
RED = \033[1;91m
GREEN = \033[1;92m
YELLOW = \033[1;93m
BLUE = \033[1;94m
MAGENTA = \033[1;95m
CYAN = \033[1;96m
WHITE = \033[1;97m
