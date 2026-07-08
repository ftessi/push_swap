# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/29 22:05:00 by umutkilicas       #+#    #+#              #
<<<<<<< HEAD
<<<<<<< HEAD
#    Updated: 2026/07/08 15:25:19 by umutkilicas      ###   ########.fr        #
=======
#    Updated: 2026/07/08 14:02:57 by ftessi           ###   ########.fr        #
>>>>>>> f1a2d09e59d0580dc167fe88bd69d05a51d3dd86
=======
#    Updated: 2026/07/08 20:59:40 by ftessi           ###   ########.fr        #
>>>>>>> 107fe90a01c265e0935ad4bfa27c6a0830daff28
#                                                                              #
# **************************************************************************** #

NAME        = push_swap

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

<<<<<<< HEAD
<<<<<<< HEAD
SRC         = algorithm_utils.c build_stack.c disorder.c ft_split.c main.c ops_push.c ops_rev_rotate.c ops_rotate.c ops_swap.c parser.c sort_three.c stack_utils.c

=======
SRC         = algorithm_utils.c  build_stack.c  disorder.c  ft_split.c  main.c  ops_push.c  ops_rev_rotate.c  ops_rotate.c  ops_swap.c  parser.c  simple.c  sort_three.c  stack_utils.c
>>>>>>> f1a2d09e59d0580dc167fe88bd69d05a51d3dd86
=======
SRC         = algorithm_utils.c  build_stack.c  disorder.c  ft_split.c  main.c  ops_push.c  ops_rev_rotate.c  ops_rotate.c  ops_swap.c  parser.c  simple.c  sort_three.c  stack_utils.c complex.c complex_utils.c
>>>>>>> 107fe90a01c265e0935ad4bfa27c6a0830daff28
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