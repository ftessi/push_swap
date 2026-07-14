# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/29 22:05:00 by umutkilicas       #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2026/07/14 13:53:44 by umutkilicas      ###   ########.fr        #
=======
#    Updated: 2026/07/13 18:00:14 by ftessi           ###   ########.fr        #
>>>>>>> 3d862e7a28cf2462ba8f37d3a3b1aeb9edde113c
#                                                                              #
# **************************************************************************** #

NAME        = push_swap

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

<<<<<<< HEAD
SRC         = algorithm_utils.c  build_stack.c  disorder.c  ft_split.c  main.c  ops_push.c  ops_rev_rotate.c  ops_rotate.c  ops_swap.c  parser.c  simple.c stack_utils.c complex.c complex_utils.c medium.c
=======
SRC         = algorithm_utils.c  build_stack.c  complex.c  complex_utils.c  disorder.c  flags.c  ft_split.c  main.c  medium.c  ops_push.c  ops_rev_rotate.c  ops_rotate.c  ops_swap.c  parser.c  simple.c  sort_three.c  stack_utils.c

>>>>>>> 3d862e7a28cf2462ba8f37d3a3b1aeb9edde113c
OBJ         = $(SRC:.c=.o)

RM          = rm -f

# Renk kodları (Terminal çıktısının güzel görünmesi için)
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