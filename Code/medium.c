/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:46:20 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/08 15:15:44 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void assign_rank(t_stack_check *stack)
{
    t_node *curr;
    t_node *compr;
    int rank;
    if (!stack || !stack->head)
        return;
    curr = stack->head; //start from the beggining tp find compr
    while(curr)
    {
       rank = 0;
       while(compr) 
    }
}

void medium_sorter(t_stack_check *a, t_stack_check *b)
{

    /*CHUNK DISTRIBUTION*/
    while(a->size > 3)
    
}