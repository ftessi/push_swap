/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:33:03 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/06 14:16:23 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_node
{
	int				value;
	int				index;
	int				pos;
	bool			median;
	int				cost;
	struct s_node	*target;
	struct s_node	*next;
	struct s_node	*prev;
}					t_node;

typedef struct s_stack_check
{
	t_node			*head;
	t_node			*tail;
	t_node			*max;
	t_node			*min;
	int				size;
	double			disorder;
}					t_stack_check;

/* --- SPLIT & PARSING --- */
char				**ft_split(char const *str, char c);
int					error_syntax(char *str);
int					error_duplicate(t_stack_check *stack, int dup);
long				ft_atoi_ps(char *str, t_stack_check *stack, char **argv,
						bool is_split);
void				free_and_exit(t_stack_check *stack, char **argv,
						bool is_split);
void				build_stack(t_stack_check *a, int argc, char **argv);


/* --- STACK UTILS --- */
t_node				*node_creator(int value);
void				node_connecter(t_stack_check *stack, t_node *new_node);
void				stack_o_meter(t_stack_check *stack);
void				free_stack(t_stack_check *stack);
bool				is_sorted(t_stack_check *stack);

/* --- ALGORITHMS AND UTILS --- */
void				three_sorter(t_stack_check *stack);
void				simple_sorter(t_stack_check *a, t_stack_check *b);

/* --- INSTRUCTIONS --- */
void				stack_pusher(t_stack_check *dest, t_stack_check *src);
void				pb(t_stack_check *a, t_stack_check *b);
void				pa(t_stack_check *a, t_stack_check *b);

void				stack_swapper(t_stack_check *stack);
void				sa(t_stack_check *a);
void				sb(t_stack_check *b);
void				ss(t_stack_check *a, t_stack_check *b);

void				stack_rotater(t_stack_check *stack);
void				ra(t_stack_check *a);
void				rb(t_stack_check *b);
void				rr(t_stack_check *a, t_stack_check *b);

void				stack_rev_rotater(t_stack_check *stack);
void				rra(t_stack_check *a);
void				rrb(t_stack_check *b);
void				rrr(t_stack_check *a, t_stack_check *b);

#endif
