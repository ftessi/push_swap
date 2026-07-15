/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:35:31 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/15 15:35:49 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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
	stack->strategy = ADAPTIVE; //  Initialize enum strategy to default
	stack->bench = false;       //  Initialize bench flag to default
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
	if (nums < 0) //  Exit immediately if an invalid flag was parsed
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