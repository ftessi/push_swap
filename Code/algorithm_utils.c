/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:10:33 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/06 15:07:38 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.c"

void    target_the_a(t_node *node_b, t_stack_check *a)
{
    t_node *curr_a;
    t_node *target_node;
    long closest_larger_value;

    curr_a = a->head;
    target_node = NULL;
    closest_larger_value = 2147483648; // Initializing above INT_MAX
    
    //loop through stack A to find closest larger neighbors
    
}