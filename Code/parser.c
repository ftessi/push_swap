/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 21:31:48 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/29 21:56:09 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int error_syntax(char *str)
{
    int i;

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
int error_duplicate(t_stack_check *stack, int dup)
{
    t_node *curr;

    if (!stack || !stack->head)
        return (0);
    curr = stack->head;

    while(curr)
    {
        if (curr->value == dup)
            return (1);
        curr =curr->next;
    }
    return (0);
}
int ft_atoi_ps(char *str, t_stack_check *stack, char **argv, bool is_split)
{
  int res;
  int sign;
  int i;
  
  res = 0;
  sign = 1;
  i = 0;

  if (str[i] == '-' || str[i] == '+')
    {
        if(str[i] == '-')
            sign = -1;
    }
    while (str[i])
    {

        res = (res * 10) + (str[i] - '0');
        res = res * sign;
        if (res > 2147483647 ||res < -2147483648)
        free_and_exit(stack, argv, is_split);
        i++;
    }
    return (res);
}

void	free_and_exit(t_stack_check *stack, char **argv, bool is_split)
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
    exit (1);
}

