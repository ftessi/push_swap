/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:15:38 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 19:23:35 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** ALGORITHM SUMMARY (simple_sorter):
**
** 1. PRE-SORT AND PUSH TO STACK B:
**    - Push elements to Stack B until Stack A has only 3 elements remaining.
**    - Update stack metrics (median, positions, min/max values) each iteration.
**    - Keep Stack B in descending order by targeting the correct node in B 
**      for A's head, rotating B until the target is at the top (top half = rb, 
**      bottom half = rrb), then pushing.
**
** 2. SOLVE BASE CASE:
**    - Sort the remaining 3 elements in Stack A using three_sorter.
**
** 3. RETURN COLLECTION (PUSH BACK TO A):
**    - For each element in Stack B, find the smallest element in Stack A that is 
**      still larger than B's head.
**    - Rotate Stack A to bring the target position to the top, then push (pa).
**
** 4. FINAL ALIGNMENT:
**    - Rotate Stack A until the absolute smallest element (min) sits at the top.
*/

void	simple_sorter(t_stack *a, t_stack *b)
{
	t_node	*target_node;

	while (a->size > 3)
	{
		stack_o_meter(a);
		stack_o_meter(b);
		target_node = target_the_b(a->head, b);
		while (b->head != target_node)
		{
			if (target_node->median == true)
				rb(b);
			else
				rrb(b);
		}
		pb(a, b);
	}

	three_sorter(a);

	while (b->size > 0)
	{
		stack_o_meter(a);
		stack_o_meter(b);
		target_node = target_the_a(b->head, a);
		while (a->head != target_node)
		{
			if (target_node->median == true)
				ra(a);
			else
				rra(a);
		}
		pa(a, b);
	}

	stack_o_meter(a);
	while (a->head != a->min)
	{
		if (a->min->median == true)
			ra(a);
		else
			rra(a);
	}
}