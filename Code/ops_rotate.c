/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 01:51:00 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/10 15:59:03 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_rotater(t_stack *stack)
{
	t_node	*old_head;
	t_node	*new_head;

	if (!stack || stack->size < 2)
		return ;
	old_head = stack->head;
	new_head = old_head->next;
	// making the new head top of the stack
	stack->head = new_head;
	new_head->prev = NULL; // cuz its new head
	// making the old head the tail
	old_head->next = NULL; // updating the next pointer of old head
	old_head->prev = stack->tail;
	// updating the prev pointer of old head to tail
	stack->tail->next = old_head; // connecting old head to tail's next pointer
	stack->tail = old_head;       // for stack_check
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