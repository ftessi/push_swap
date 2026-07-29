/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:35:31 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 23:07:23 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** STACK INITIALIZATION & BUILDING SUMMARY:
**
** 1. init_stack:
**    - Allocates memory for a new t_stack structure and sets default values
**      for pointers, operation counters, strategy (ADAPTIVE), and bench flag.
**
** 2. free_tokens:
**    - Frees a null-terminated array of string tokens allocated during split.
**
** 3. add_value:
**    - Validates syntax, converts string to integer, checks for duplicates,
**      creates a node, and appends it to stack A. Exits gracefully on error.
**
** 4. parse_split:
**    - Splits a single space-separated string argument into tokens, parses
**      each value into stack A, and frees allocated memory.
**
** 5. build_stack:
**    - Main initialization entry point: parses flags, processes input arguments
**      (either a single space-separated string or multiple command-line args),
**      and populates stack A.
*/

t_stack	*init_stack(void)
{
	t_stack	*stack;

	stack = (t_stack *)malloc(sizeof(t_stack));
	if (!stack)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->max = NULL;
	stack->min = NULL;
	stack->size = 0;
	stack->disorder = 0.0;
	stack->op_sa = 0;
	stack->op_sb = 0;
	stack->op_ss = 0;
	stack->op_pa = 0;
	stack->op_pb = 0;
	stack->op_ra = 0;
	stack->op_rb = 0;
	stack->op_rr = 0;
	stack->op_rra = 0;
	stack->op_rrb = 0;
	stack->op_rrr = 0;
	stack->strategy = ADAPTIVE;
	stack->bench = false;
	return (stack);
}

static void	free_tokens(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	add_value(t_stack *a, char *tok, char **arr, bool is_split)
{
	int		value;
	t_node	*node;

	if (error_syntax(tok))
		free_and_exit(a, arr, is_split);
	value = (int)ft_atoi_ps(tok, a, arr, is_split);
	if (error_duplicate(a, value))
		free_and_exit(a, arr, is_split);
	node = node_creator(value);
	if (!node)
		free_and_exit(a, arr, is_split);
	node_connecter(a, node);
}

static void	parse_split(t_stack *a, char *arg)
{
	char	**arr;
	int		i;

	arr = ft_split(arg, ' ');
	if (!arr)
		free_and_exit(a, NULL, false);
	i = 0;
	while (arr[i])
	{
		add_value(a, arr[i], arr, true);
		i++;
	}
	free_tokens(arr);
}

void	build_stack(t_stack *a, int argc, char **argv)
{
	int	i;
	int	nums;

	nums = parse_flags(a, argc, argv);
	if (nums < 0)
		free_and_exit(a, NULL, false);
	if (nums == 0)
		return ;
	if (nums == 1)
		parse_split(a, first_number(argc, argv));
	else
	{
		i = 1;
		while (i < argc)
		{
			if (!is_flag_token(argv[i]))
				add_value(a, argv[i], NULL, false);
			i++;
		}
	}
	if (a->size == 0)
		free_and_exit(a, NULL, false);
}
