/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:41:02 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 23:06:32 by ftessi           ###   ########.fr       */
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
**    - Assigns ranks, derives the chunk size from sqrt(size), distributes to
**      B, sorts the remaining 3 in A, then recovers all elements to A.
**
** COMPLEXITY (in generated push_swap operations):
**    - chunk_size = 1.5*sqrt(n) gives ~sqrt(n) chunks of ~sqrt(n) elements.
**      The 1.5 factor is a measured constant (it beat 1.0 and 2.0 at both
**      n=100 and n=500); it does not change the complexity class.
**    - Distribution: each element is pushed once and A is rotated at most
**      once per element per chunk pass -> O(n*sqrt(n)).
**    - Recovery: each element is pushed back once, and B is rotated at most
**      O(sqrt(n)) times to expose the next maximum -> O(n*sqrt(n)).
**    - Upper bound O(n*sqrt(n)), space O(n) (n nodes, no auxiliary array).
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
		else if (a->head->index < (total_size - 3) && a->head->index <= (idx
				+ chunk_size))
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
	chunk_size = int_sqrt(a->size) * 3 / 2;
	send_to_b(a, b, chunk_size);
	three_sorter(a);
	return_to_a(a, b);
}
