/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:33:03 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/13 18:01:29 by ftessi           ###   ########.fr       */
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

typedef enum e_strategy
{
	ADAPTIVE,
	SIMPLE,
	MEDIUM,
	COMPLEX
}					t_strategy;

typedef struct s_stack_check
{
	t_node			*head;
	t_node			*tail;
	t_node			*max;
	t_node			*min;
	int				size;
	double			disorder;
	int				op_sa;
	int				op_sb;
	int				op_ss;
	int				op_pa;
	int				op_pb;
	int				op_ra;
	int				op_rb;
	int				op_rr;
	int				op_rra;
	int				op_rrb;
	int				op_rrr;
	t_strategy		strategy;
	bool			bench;
}					t_stack;

/* --- FLAGS --- */
int is_flag_token(char  *arg);
int parse_flags(t_stack *a, int argc, char **argv);
char    *first_number(int argc, char **argv);

/* --- SPLIT & PARSING --- */
char				**ft_split(char const *str, char c);
int					error_syntax(char *str);
int					error_duplicate(t_stack *stack, int dup);
long				ft_atoi_ps(char *str, t_stack *stack, char **argv,
						bool is_split);
void				free_and_exit(t_stack *stack, char **argv, bool is_split);
void				build_stack(t_stack *a, int argc, char **argv);

/* --- STACK UTILS --- */
t_node				*node_creator(int value);
void				node_connecter(t_stack *stack, t_node *new_node);
void				stack_o_meter(t_stack *stack);
void				free_stack(t_stack *stack);
bool				is_sorted(t_stack *stack);
double				compute_disorder(t_stack *stack);
t_node				*target_the_a(t_node *node_b, t_stack *a);
t_node				*target_the_b(t_node *node_a, t_stack *b);
t_stack				*init_stack(void);

/* --- ALGORITHMS AND UTILS --- */
void				three_sorter(t_stack *stack);
void				simple_sorter(t_stack *a, t_stack *b);
void				medium_sorter(t_stack *a, t_stack *b);
void				complex_sorter(t_stack *a, t_stack *b);
void				assign_cost(t_stack *a, t_stack *b);
t_node				*find_cheapest(t_stack *b);
int					signed_rot(int pos, int size);

/* --- INSTRUCTIONS --- */
void				stack_pusher(t_stack *dest, t_stack *src);
void				pb(t_stack *a, t_stack *b);
void				pa(t_stack *a, t_stack *b);

void				stack_swapper(t_stack *stack);
void				sa(t_stack *a);
void				sb(t_stack *b);
void				ss(t_stack *a, t_stack *b);

void				stack_rotater(t_stack *stack);
void				ra(t_stack *a);
void				rb(t_stack *b);
void				rr(t_stack *a, t_stack *b);

void				stack_rev_rotater(t_stack *stack);
void				rra(t_stack *a);
void				rrb(t_stack *b);
void				rrr(t_stack *a, t_stack *b);

#endif
