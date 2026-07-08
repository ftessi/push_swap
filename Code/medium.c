/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:46:20 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/08 20:04:14 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	assign_rank(t_stack_check *stack)
{
	t_node	*curr;
	t_node	*compr;
	int		rank;

	if (!stack || !stack->head)
		return ;
	curr = stack->head; // start from the beggining tp find compr
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

void	medium_sorter(t_stack_check *a, t_stack_check *b)
{
	int chunk_size;
	int idx;
	t_node *target_node;

	assign_rank(a);

	if (a->size <= 100)
		chunk_size = 18; // ideal size for 100 elements
	else
		chunk_size = 45; // better for 500
	idx = 0;             // index

	/*CHUNK DISTRIBUTION*/
	while (a->size > 3)
	{
		if (a->head->index <= idx)
		{
			pb(a, b);
			rb(b); // send the smallest chunk elements to bottom of B
			idx++;
		}
		else if (a->head->index <= (idx + chunk_size))
		{
			pb(a, b); // keep the middle chunk at the top of the B
			idx++;
		}
		else
			ra(a);
	}
	/*SOLVE BASE THREE*/
	three_sorter(a);

	/*ASCENDING RETURN RECOVRY (B -> A)*/
	while (b->size > 0)
	{
		stack_o_meter(a);
		stack_o_meter(b);
		// Find the absolute closest ascending landing target in A
		target_node = target_the_a(b->head, a);
		// rotate stack A until its landing destiantion is exposed at the top row
		while (target_node->pos != 0)
		{
			if (target_node->median == true)
				ra(a);
			else
				rra(a);
		}
		pa(a, b);
	}

	/*RE_ALIGN SMALLEST BOUNDRY*/
	stack_o_meter(a);
	while (a->min->pos != 0)
	{
		if (a->min->median == true)
			ra(a);
		else
			rra(a);
		stack_o_meter(a);
	}
}