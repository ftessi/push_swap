# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/29 22:05:00 by umutkilicas       #+#    #+#              #
#    Updated: 2026/07/14 17:22:14 by umutkilicas      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = push_swap

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

SRC         = algorithm_utils.c build_stack.c disorder.c ft_split.c main.c \
              ops_push.c ops_rev_rotate.c ops_rotate.c ops_swap.c parser.c \
              simple.c stack_utils.c complex.c complex_utils.c medium.c    \
			  

OBJ         = $(SRC:.c=.o)

RM          = rm -f

GREEN       = \033[0;32m
YELLOW      = \033[0;33m
RESET       = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(NAME) successfully created!$(RESET)"

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	$(RM) $(OBJ)
	@echo "$(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Cleaning binary...$(RESET)"
	$(RM) $(NAME)
	@echo "$(GREEN)All cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re