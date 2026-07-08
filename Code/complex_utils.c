/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 20:18:16 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/08 20:43:55 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	signed_rot(int pos, int size)
{
	if (pos * 2 <= size)
		return (pos);
	return (pos - size);
}

static int	abs_i(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}

void	assign_cost(t_stack_check *a, t_stack_check *b)
{
	t_node	*curr;
	int		ca;
	int		cb;

	stack_o_meter(a);
	stack_o_meter(b);
	curr = b->head;
	while (curr)
	{
		curr->target = target_the_a(curr, a);
		cb = signed_rot(curr->pos, b->size);
		ca = signed_rot(curr->target->pos, a->size);
		if ((ca >= 0) == (cb >= 0) && abs_i(ca) > abs_i(cb))
			curr->cost = abs_i(ca);
		else if ((ca >= 0) == (cb >= 0))
			curr->cost = abs_i(cb);
		else
			curr->cost = abs_i(ca) + abs_i(cb);
		curr = curr->next;
	}
}

t_node	*find_cheapest(t_stack_check *b)
{
	t_node *curr;
	t_node *best;

	best = b->head;
	curr = b->head;
	while (curr)
	{
		if (curr->cost < best->cost)
			best = curr;
		curr = curr->next;
	}
	return (best);
}