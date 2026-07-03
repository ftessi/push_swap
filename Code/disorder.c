/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 20:56:21 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/03 19:21:29 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	compute_disorder(t_stack_check *stack)
{
	t_node *i;
	t_node *j;
	double mistakes;
	double total_pairs;

	if (!stack || stack->size < 2)
		mistakes = 0.0;
	total_pairs = 0.0;
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
	if (mistakes == 0.0)
		return (0.0);
	return (mistakes / total_pairs);
}