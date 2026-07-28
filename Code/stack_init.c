/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 10:31:47 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/29 10:31:47 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** STACK ALLOCATION:
**
** 1. reset_counters:
**    - Zeroes the 11 operation counters. Split out of init_stack so both
**      stay inside the 25-line norm limit and so the counter list has a
**      single home when a new operation is ever added.
**
** 2. init_stack:
**    - Allocates a t_stack and puts it in a known-empty state: no nodes, no
**      min/max, zero disorder, ADAPTIVE selection, NONE executed, no bench.
**    - Returns NULL on allocation failure; the caller reports the error.
*/

static void	reset_counters(t_stack *stack)
{
	stack->op_sa = 0;
	stack->op_sb = 0;
	stack->op_ss = 0;
	stack->op_pa = 0;
	stack->op_pb = 0;
	stack->op_ra = 0;
	stack->op_rb = 0;
	stack->op_rr = 0;
	stack->op_rra = 0;
	stack->op_rrb = 0;
	stack->op_rrr = 0;
}

t_stack	*init_stack(void)
{
	t_stack	*stack;

	stack = (t_stack *)malloc(sizeof(t_stack));
	if (!stack)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->max = NULL;
	stack->min = NULL;
	stack->size = 0;
	stack->disorder = 0.0;
	reset_counters(stack);
	stack->strategy = ADAPTIVE;
	stack->resolved = NONE;
	stack->bench = false;
	return (stack);
}
