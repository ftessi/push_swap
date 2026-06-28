/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rev_rotate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 02:15:23 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/28 02:41:56 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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