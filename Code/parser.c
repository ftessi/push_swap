/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 21:31:48 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/10 15:59:03 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	error_syntax(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (1);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	error_duplicate(t_stack *stack, int dup)
{
	t_node	*curr;

	if (!stack || !stack->head)
		return (0);
	curr = stack->head;
	while (curr)
	{
		if (curr->value == dup)
			return (1);
		curr = curr->next;
	}
	return (0);
}

long	ft_atoi_ps(char *str, t_stack *stack, char **argv, bool is_split)
{
	long	res;
	int		sign;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			sign = -1;
			i++;
		}
	}
	while (str[i])
	{
		res = (res * 10) + (str[i] - '0');
		if (res * sign > 2147483647 || res * sign < -2147483648)
			free_and_exit(stack, argv, is_split);
		i++;
	}
	return (res * sign);
}

void	free_and_exit(t_stack *stack, char **argv, bool is_split)
{
	int	i;

	if (argv && is_split)
	{
		i = 0;
		while (argv[i])
		{
			free(argv[i]);
			i++;
		}
		free(argv);
	}
	if (stack)
		free_stack(stack);
	write(2, "Error\n", 6);
	exit(1);
}
