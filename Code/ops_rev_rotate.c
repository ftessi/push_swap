/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rev_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukilicas <ukilicas@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 02:15:23 by ukilicas          #+#    #+#             */
/*   Updated: 2026/08/04 14:48:38 by ukilicas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_rev_rotater(t_stack *stack)
{
	t_node	*old_tail;
	t_node	*new_tail;

	if (!stack || stack->size < 2)
		return ;
	old_tail = stack->tail;
	new_tail = old_tail->prev;
	stack->tail = new_tail;
	new_tail->next = NULL;
	old_tail->prev = NULL;
	old_tail->next = stack->head;
	stack->head->prev = old_tail;
	stack->head = old_tail;
}

void	rra(t_stack *a)
{
	stack_rev_rotater(a);
	a->op_rra++;
	write(1, "rra\n", 4);
}

void	rrb(t_stack *b)
{
	stack_rev_rotater(b);
	b->op_rrb++;
	write(1, "rrb\n", 4);
}

void	rrr(t_stack *a, t_stack *b)
{
	stack_rev_rotater(a);
	stack_rev_rotater(b);
	a->op_rrr++;
	write(1, "rrr\n", 4);
}
