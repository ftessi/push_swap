/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_swap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 03:29:48 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 22:52:32 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** SWAP OPERATIONS SUMMARY:
**
** 1. stack_swapper:
**    - Swaps the top two elements of a stack (e.g., A -> B -> C becomes B -> A
	-> C).
**    - Adjusts head, tail, next, and prev pointers to safely re-link the nodes.
**    - Does nothing if stack is NULL or contains fewer than 2 elements.
**
** 2. sa, sb, ss:
**    - sa: Swaps the first 2 elements at top of Stack A, increments counter,
	prints "sa\n".
**    - sb: Swaps the first 2 elements at top of Stack B, increments counter,
	prints "sb\n".
**    - ss: Swaps top 2 elements of both Stack A and Stack B,
	increments counter, prints "ss\n".
*/

void	stack_swapper(t_stack *stack)
{
	t_node	*first;
	t_node	*second;

	if (!stack || stack->size < 2)
		return ;
	first = stack->head;
	second = first->next;
	first->next = second->next;
	first->prev = second;
	if (second->next)
		second->next->prev = first;
	else
		stack->tail = first;
	second->next = first;
	second->prev = NULL;
	stack->head = second;
}

void	sa(t_stack *a)
{
	stack_swapper(a);
	a->op_sa++;
	write(1, "sa\n", 3);
}

void	sb(t_stack *b)
{
	stack_swapper(b);
	b->op_sb++;
	write(1, "sb\n", 3);
}

void	ss(t_stack *a, t_stack *b)
{
	stack_swapper(a);
	stack_swapper(b);
	a->op_ss++;
	write(1, "ss\n", 3);
}
