/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 20:02:55 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/29 22:02:36 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void three_sorter(t_stack_check *stack)
{
    stack_o_meter(stack);

    if (stack->max->pos == 0)//if max on top but to bottom
        ra(stack);
    else if (stack->max->pos == 1)//if max on the middle put the bottom
        rra(stack);
    if (stack->head->value > stack->head->next->value)
        sa(stack); 
}