/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:10:33 by umutkilicas       #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2026/07/14 13:53:26 by umutkilicas      ###   ########.fr       */
=======
/*   Updated: 2026/07/14 16:04:58 by ftessi           ###   ########.fr       */
>>>>>>> 9cb81f1ba54e47589840a5dbc2984c342af3603b
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*target_the_a(t_node *node_b, t_stack *a)
{
	t_node	*curr_a;
	t_node	*target_node;
	long	closest_larger_value;

	curr_a = a->head;
	target_node = NULL;
	closest_larger_value = 2147483648LL; // Initializing above INT_MAX
	// loop through stack A to find closest larger neighbor
	while (curr_a)
	{
		if (curr_a->value > node_b->value
			&& curr_a->value < closest_larger_value)
		{
			closest_larger_value = curr_a->value;
			target_node = curr_a;
		}
		curr_a = curr_a->next;
	}
	if (target_node == NULL)
		return (a->min);
	return (target_node);
}

t_node	*target_the_b(t_node *node_a, t_stack *b)
{
	t_node	*curr_b;
	t_node	*target_node;
	long	closest_smaller_value;

	curr_b = b->head;
	target_node = NULL;
	closest_smaller_value = -2147483649LL; // Initializing above INT_MIN
	// loop through stack B to find closest larger neighbor
	while (curr_b)
	{ //  look for a number in B smaller than A but closest to a
		if (curr_b->value < node_a->value
			&& curr_b->value > closest_smaller_value)
		{
			closest_smaller_value = curr_b->value;
			target_node = curr_b;
		}
		curr_b = curr_b->next;
	}
	if (target_node == NULL)
		return (b->max);
	return (target_node);
}
void three_sorter(t_stack *stack)
{
    stack_o_meter(stack);

    if (stack->max->pos == 0)//if max on top put to bottom
        ra(stack);
    else if (stack->max->pos == 1)//if max on the middle put the bottom
        rra(stack);
    if (stack->head->value > stack->head->next->value)
        sa(stack); 
}