/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 20:56:21 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/10 15:59:03 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	compute_disorder(t_stack *stack)
{
	t_node *i;
	t_node *j;
	double mistakes;
	double total_pairs;

	mistakes = 0.0;
	total_pairs = 0.0;
	if (!stack || stack->size < 2)
		return (0.0);
	i = stack->head;
	while (i)
	{
		j = i->next;
		while (j)
		{
			total_pairs += 1.0;
			if (i->value > j->value)
				mistakes += 1.0;
			j = j->next;
		}
		i = i->next;
	}
	return (mistakes / total_pairs);
}