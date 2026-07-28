/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 20:37:59 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/28 22:48:26 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** COMPLEX SORTER SUMMARY:
**
** 1. rotate_common:
**    - Executes simultaneous rotations (rr or rrr) on both stacks when
**      rotation directions for the chosen B node and its target A node match.
**
** 2. rotate_rest:
**    - Executes remaining individual rotations (ra, rra, rb, rrb) after
**      common rotations have been exhausted.
**
** 3. move_cheapest:
**    - Calculates signed rotation distances for a node in Stack B and its target in Stack A.
**    - Applies common and individual rotations to align both nodes, then pushes to Stack A (pa).
**
** 4. final_rotate:
**    - Rotates Stack A until the absolute minimum element sits at the top.
**
** 5. complex_sorter:
**    - Cost-based (Turk algorithm) sorter: pushes elements to B until only 3 remain in A,
**      sorts the 3 elements in A, iteratively finds and moves the cheapest node from B
**      to its target position in A, and performs final alignment.
*/

static void	rotate_common(t_stack *a, t_stack *b, int *ca, int *cb)
{
	while (*ca > 0 && *cb > 0)
	{
		rr(a, b);
		(*ca)--;
		(*cb)--;
	}
	while (*ca < 0 && *cb < 0)
	{
		rrr(a, b);
		(*ca)++;
		(*cb)++;
	}
}

static void	rotate_rest(t_stack *a, t_stack *b, int ca, int cb)
{
	while (ca > 0)
	{
		ra(a);
		ca--;
	}
	while (ca < 0)
	{
		rra(a);
		ca++;
	}
	while (cb > 0)
	{
		rb(b);
		cb--;
	}
	while (cb < 0)
	{
		rrb(b);
		cb++;
	}
}

static void	move_cheapest(t_stack *a, t_stack *b, t_node *bn)
{
	int	ca;
	int	cb;

	stack_o_meter(a);
	stack_o_meter(b);
	cb = signed_rot(bn->pos, b->size);
	ca = signed_rot(bn->target->pos, a->size);
	rotate_common(a, b, &ca, &cb);
	rotate_rest(a, b, ca, cb);
	pa(a, b);
}

static void	final_rotate(t_stack *a)
{
	stack_o_meter(a);
	if (a->min->pos * 2 <= a->size)
		while (a->head != a->min)
			ra(a);
	else
		while (a->head != a->min)
			rra(a);
}

void	complex_sorter(t_stack *a, t_stack *b)
{
	t_node *cheapest;

	while (a->size > 3)
		pb(a, b);
	three_sorter(a);
	while (b->size > 0)
	{
		assign_cost(a, b);
		cheapest = find_cheapest(b);
		move_cheapest(a, b, cheapest);
	}
	final_rotate(a);
}