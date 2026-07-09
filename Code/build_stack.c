/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:31:39 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/09 14:33:05 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack_check	*init_stack(void)
{
	t_stack_check	*stack;

	stack = malloc(sizeof(t_stack_check));
	if (!stack)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->max = NULL;
	stack->min = NULL;
	stack->size = 0;
	stack->disorder = 0.0;
	stack->op_sa = 0;
	stack->op_pa = 0;
	stack->op_pb = 0;
	stack->op_ra = 0;
	stack->op_rb = 0;
	stack->op_rr = 0;
	stack->op_rra = 0;
	stack->op_rrb = 0;
	stack->op_rrr = 0;
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

static void	add_value(t_stack_check *a, char *tok, char **arr, bool is_split)
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

static void	parse_split(t_stack_check *a, char *arg)
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

void	build_stack(t_stack_check *a, int argc, char **argv)
{
	int i;

	if (argc == 2)
		parse_split(a, argv[1]);
	else
	{
		i = 1;
		while (i < argc)
		{
			add_value(a, argv[i], NULL, false);
			i++;
		}
	}
	if (a->size == 0)
		free_and_exit(a, NULL, false);
}