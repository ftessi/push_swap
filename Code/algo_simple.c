/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:15:38 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/14 16:08:23 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

void	simple_sorter(t_stack *a, t_stack *b)
{
	t_node *target_node;

	/*====PRE-SORT AND PUSH TO STACK B====*/

	while (a->size > 3) // we have to push till stack a has 3 elements
	{
		stack_o_meter(a); // update the metrics of the stacks
		stack_o_meter(b); // median, positions, min and max value

		/*we can not throw the elements to b randomly
		b should be always in desencding order
		thats why find the target node from b for the top of a
		*/
		target_node = target_the_b(a->head, b);
		// rotate the stack b until target node sitting at the top (pos == 0)
		while (b->head != target_node)
		{
			if (target_node->median == true)
				rb(b); // rotate up if it's in top half
			else
				rrb(b); // rev rotate it it's in bottom half
		}
		// once target spot is open at top, push it
		pb(a, b);
	}
	/*====SOLVE THE BASE CASE====*/
	// now there is just 3 elements on the a
	three_sorter(a);

	/*====RETURN THE COLLECTION (PUSH BACK TO A)====*/
	while (b->size > 0)
	{
		stack_o_meter(a); // update
		stack_o_meter(b);

		// stack A sorted in ascending order, so find the smaltest in the A
		//  that is still larger than b->head
		target_node = target_the_a(b->head, a);
		// rotate the stack A until target node sits at top
		while (a->head != target_node)
		{
			if (target_node->median == true)
				ra(a);
			else
				rra(a);
		}
		pa(a, b); // push it to A
	}

	/*====FINAL ALIGNMENT====*/
	stack_o_meter(a);

	// bring the absolute smallest element to top of the complete sort
	while (a->head != a->min)
	{
		if (a->min->median == true)
			ra(a);
		else
			rra(a);
	}
}