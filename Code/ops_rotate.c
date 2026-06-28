/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 01:51:00 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/28 03:03:59 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void stack_rotater (t_stack_check *stack)
{
    t_node *old_head;
    t_node *new_head;

    if (!stack || stack->size < 2)
        return ;
    old_head = stack->head;
    new_head = old_head->next;
//making the new head top of the stack
    stack->head = new_head;
    new_head->prev = NULL;//cuz its new head
//making the old head the tail
    old_head->next = NULL;//updating the next pointer of old head
    old_head->prev = stack->tail;//updating the prev pointer of old head to tail
    stack->tail->next = old_head; //connecting old head to tail's next pointer

    stack->tail = old_head;//for stack_check 
}