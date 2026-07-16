/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 14:41:02 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/16 14:41:05 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "push_swap.h"

/*
** ==== COGNITIVE RANK ASSIGNMENT ====
** To make our chunk sorting process smooth and independent of actual numbers 
** (which could be negative, very large, or far apart), we scan the stack 
** and assign a relative "index/rank" from 0 to (size - 1) to each value.
** Example: If stack is [42, -5, 100], their assigned indexes will be [1, 0, 2].
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

/*
** ==== FIND THE ABSOLUTE MAXIMUM IN B ====
** During the recovery phase (B -> A), we want to push elements back in 
** descending order. To do this with maximum efficiency, we must always target 
** the node with the highest index currently sitting in stack B.
*/
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

/*
** ==== CHUNK-BASED DISTRIBUTION (A -> B) ====
** We distribute elements from A to B using a dynamic window (chunk).
** CRITICAL FIX: We must ensure that only the absolute 3 largest elements 
** (indices >= total_size - 3) are left in stack A. 
** To do this, we protect them by ensuring we only push nodes to B if their 
** index is strictly less than (total_size - 3). If we encounter one of the 
** 3 largest elements, we skip it (ra) and keep searching for smaller nodes.
*/
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

/*
** ==== EFFICIENT RECOVERY PHASE (B -> A) ====
** Since stack A was left with only the 3 largest elements sorted in ascending order,
** we can push everything from B back to A in perfect sorted order without 
** ever rotating stack A! 
** We find B's max node, rotate B until it reaches the top (pos == 0), 
** and push it directly to A.
*/
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

/*
** ==== MAIN ENTRY FOR COMPLEX SORTER ====
** This function handles complex sorting for stacks bigger than 3 elements.
** - We first index all nodes to operate on relative ranks.
** - We select the mathematically optimal chunk sizes (12 for 100, 30 for 500).
** - We push nodes to B in layered chunks, sort the remaining 3 in A, 
** and gracefully return B's elements back to A in perfect descending order.
*/
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