/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 20:37:59 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/14 16:05:08 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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