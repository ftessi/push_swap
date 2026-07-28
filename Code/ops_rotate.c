/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 01:51:00 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 19:58:20 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** ROTATE OPERATIONS SUMMARY:
**
** 1. stack_rotater:
**    - Shifts all elements of a stack up by 1 position.
**    - The first element (head) becomes the last element (tail).
**    - Re-links stack pointers (head, tail, next, prev) to maintain integrity.
**
** 2. ra, rb, rr:
**    - ra: Rotates Stack A, increments operation counter, prints "ra\n".
**    - rb: Rotates Stack B, increments operation counter, prints "rb\n".
**    - rr: Rotates both Stack A and Stack B, increments counter, prints "rr\n".
*/

void	stack_rotater(t_stack *stack)
{
	t_node	*old_head;
	t_node	*new_head;

	if (!stack || stack->size < 2)
		return ;
	old_head = stack->head;
	new_head = old_head->next;
	stack->head = new_head;
	new_head->prev = NULL;
	old_head->next = NULL;
	old_head->prev = stack->tail;
	stack->tail->next = old_head;
	stack->tail = old_head;
}

void	ra(t_stack *a)
{
	stack_rotater(a);
	a->op_ra++;
	write(1, "ra\n", 3);
}

void	rb(t_stack *b)
{
	stack_rotater(b);
	b->op_rb++;
	write(1, "rb\n", 3);
}

void	rr(t_stack *a, t_stack *b)
{
	stack_rotater(a);
	stack_rotater(b);
	a->op_rr++;
	write(1, "rr\n", 3);
}