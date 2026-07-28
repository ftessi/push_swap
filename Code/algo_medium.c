/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:41:02 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 19:34:59 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "push_swap.h"

/*
** ALGORITHM MEDIUM SUMMARY:
**
** 1. assign_rank:
**    - Scans Stack A and assigns relative 0-indexed ranks (0 to size - 1)
**      to work independently of actual values (negative, huge, or spread out).
**
** 2. find_max_node:
**    - Scans Stack B and returns the node with the highest assigned index.
**
** 3. send_to_b:
**    - Distributes elements from A to B using dynamic chunk ranges.
**    - Keeps the 3 largest elements (index >= total_size - 3) in Stack A,
**      pushing smaller nodes to B in layered chunks.
**
** 4. return_to_a:
**    - Finds the maximum element in Stack B, rotates B until it reaches top,
**      and pushes back to Stack A in perfect descending order.
**
** 5. medium_sorter:
**    - Main entry point for medium/large stack sorting (>3 elements).
**    - Assigns ranks, selects chunk size (12 for <= 100, 30 for > 100),
**      distributes to B, sorts remaining 3 in A, and recovers all elements to A.
*/

static void	assign_rank(t_stack *stack)
{
	t_node	*curr;
	t_node	*compr;
	int		rank;

	if (!stack || !stack->head)
		return ;
	curr = stack->head;
	while (curr)
	{
		rank = 0;
		compr = stack->head;
		while (compr)
		{
			if (curr->value > compr->value)
				rank++;
			compr = compr->next;
		}
		curr->index = rank;
		curr = curr->next;
	}
}

static t_node	*find_max_node(t_stack *b)
{
	t_node	*curr;
	t_node	*max_node;

	if (!b || !b->head)
		return (NULL);
	curr = b->head;
	max_node = curr;
	while (curr)
	{
		if (curr->index > max_node->index)
			max_node = curr;
		curr = curr->next;
	}
	return (max_node);
}

static void	send_to_b(t_stack *a, t_stack *b, int chunk_size)
{
	int	idx;
	int	total_size;

	idx = 0;
	total_size = a->size;
	while (a->size > 3)
	{
		if (a->head->index < (total_size - 3) && a->head->index <= idx)
		{
			pb(a, b);
			rb(b);
			idx++;
		}
		else if (a->head->index < (total_size - 3)
			&& a->head->index <= (idx + chunk_size))
		{
			pb(a, b);
			idx++;
		}
		else
			ra(a);
	}
}

static void	return_to_a(t_stack *a, t_stack *b)
{
	t_node	*max_node;

	while (b->size > 0)
	{
		stack_o_meter(b);
		max_node = find_max_node(b);
		while (max_node->pos != 0)
		{
			if (max_node->median == true)
				rb(b);
			else
				rrb(b);
			stack_o_meter(b);
		}
		pa(a, b);
	}
}

void	medium_sorter(t_stack *a, t_stack *b)
{
	int	chunk_size;

	assign_rank(a);
	if (a->size <= 100)
		chunk_size = 12;
	else
		chunk_size = 30;
	send_to_b(a, b, chunk_size);
	three_sorter(a);
	return_to_a(a, b);
}