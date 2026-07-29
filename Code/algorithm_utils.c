/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:10:33 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 23:01:40 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** ALGORITHM UTILS SUMMARY:
**
** 1. target_the_a:
**    - Finds the node in Stack A that has the closest larger value than node_b.
**    - Uses 2147483648LL (above INT_MAX) as the initial boundary.
**    - If no larger value exists in Stack A,
	returns the minimum element (a->min).
**
** 2. target_the_b:
**   
	- Finds the node in Stack B that has the closest smaller value than node_a.
**    - Uses -2147483649LL (below INT_MIN) as the initial boundary.
**    - If no smaller value exists in Stack B,
	returns the maximum element (b->max).
**
** 3. three_sorter:
**    - Sorts 3 elements in Stack A in ascending order (max 2 operations).
**    - Updates stack metrics, then positions the max value at the bottom
**      (ra if max is at top pos 0, rra if max is at middle pos 1).
**    - Performs sa if the top two elements are still out of order.
*/

t_node	*target_the_a(t_node *node_b, t_stack *a)
{
	t_node	*curr_a;
	t_node	*target_node;
	long	closest_larger_value;

	curr_a = a->head;
	target_node = NULL;
	closest_larger_value = 2147483648LL;
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
	closest_smaller_value = -2147483649LL;
	while (curr_b)
	{
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

void	three_sorter(t_stack *stack)
{
	stack_o_meter(stack);
	if (stack->max->pos == 0)
		ra(stack);
	else if (stack->max->pos == 1)
		rra(stack);
	if (stack->head->value > stack->head->next->value)
		sa(stack);
}
