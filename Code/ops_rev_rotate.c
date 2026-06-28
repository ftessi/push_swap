/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rev_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 02:15:23 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/28 04:12:49 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void    stack_rev_rotater(t_stack_check *stack)
{
    t_node *old_tail;
    t_node *new_tail;

    if (!stack || stack->size < 2)
        return ;
    old_tail = stack->tail;
    new_tail = old_tail->prev;
    //making the new tail end of the stack
    stack->tail = new_tail;
    new_tail->next = NULL;
    //making the old tail head of the stack
    old_tail->prev = NULL;
    old_tail->next = stack->head;
    stack->head->prev = old_tail;
    stack->head = old_tail;
}   

void rra(t_stack_check *a)
{
    stack_rev_rotater (a);
    write(1, "rra\n", 4);
}

void rrb(t_stack_check *b)
{
    stack_rev_rotater (b);
    write(1, "rrb\n", 4);
}
void rrr(t_stack_check *a, t_stack_check *b)
{
    stack_rev_rotater (a);
    stack_rev_rotater (b);
    write(1, "rrr\n", 4);
}