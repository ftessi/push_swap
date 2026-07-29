/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:15:38 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/29 14:19:02 by umutkilicas      ###   ########.fr       */
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
**    - For each element in Stack B,
	find the smallest element in Stack A that is
**      still larger than B's head.
**    - Rotate Stack A to bring the target position to the top, then push (pa).
**
** 4. FINAL ALIGNMENT:
**    - Rotate Stack A until the absolute smallest element (min) sits at the
**      top, so the result reads ascending from the top of A.
**
** COMPLEXITY (in generated push_swap operations):
**    - Each of the n elements is placed by one scan plus at most n/2 rotations
**      to reach its insertion point, on both the way out and the way back.
**    - Upper bound O(n^2). This is the deliberate O(n^2) baseline required by
**      the subject; it is not meant to be competitive with medium/complex.
*/

static void	push_to_b(t_stack *a, t_stack *b)
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
}

static void	push_to_a(t_stack *a, t_stack *b)
{
	t_node	*target_node;

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
}

static void	final_rotate(t_stack *a)
{
	stack_o_meter(a);
	while (a->head != a->min)
	{
		if (a->min->median == true)
			ra(a);
		else
			rra(a);
	}
}

void	simple_sorter(t_stack *a, t_stack *b)
{
	push_to_b(a, b);
	three_sorter(a);
	push_to_a(a, b);
	final_rotate(a);
}
