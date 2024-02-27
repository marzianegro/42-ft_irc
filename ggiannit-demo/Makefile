###### VARIABLE ######

NAME_S = server
NAME_C = client

SRC = server.cpp client.cpp

OBJ = $(SRC:.cpp=.o)

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

%.o: %.cpp
	@echo "$(GREEN)Compiling:$(BLUE) $< $(DEF_COLOR)"
	@$(CC) $(CFLAGS) -c $< -o $@

####### RULES ########

all: $(NAME_S) $(NAME_C)

$(NAME_S): $(OBJ)
	@echo "$(MAGENTA)Program: $(WHITE)$(CC) $(CFLAGS) -o $(NAME_S) $(OBJ) $(DEF_COLOR)"
	@$(CC) $(CFLAGS) -o $(NAME_S) server.o
	@echo "$(MAGENTA)Program name: $(WHITE)$(NAME_S)$(DEF_COLOR)"

$(NAME_C): $(OBJ)
	@echo "$(MAGENTA)Program: $(WHITE)$(CC) $(CFLAGS) -o $(NAME_C) $(OBJ) $(DEF_COLOR)"
	@$(CC) $(CFLAGS) -o $(NAME_C) client.o
	@echo "$(MAGENTA)Program name: $(WHITE)$(NAME_C)$(DEF_COLOR)"

clean:
	@rm -f $(OBJ)
	@echo "$(RED)Objects removed: $(WHITE)$(OBJ)$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME_S) $(NAME_C) 
	@echo "$(RED)Program removed: $(WHITE)$(NAME_S) $(NAME_C)$(DEF_COLOR)"

re: fclean all

.PHONY: all $(NAME_S) $(NAME_C) clean fclean re

###### COLORS ########

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
