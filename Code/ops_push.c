/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 00:51:13 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/28 01:50:37 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void stack_pusher (t_stack_check *dest, t_stack_check *src)
{
    t_node  *node_to_move;

    if (!src->head)
        return ;
    node_to_move = src->head;//isolating the source head
    src->head = src->head->next;
    if (src->head)
        src->head->prev = NULL;
    else
        src->tail = NULL;
    src->size = src->size-1;

    node_to_move->next = dest->head;

    if (dest->head)
        dest->head->prev = node_to_move;
    else
        dest->tail = node_to_move;

    dest->head = node_to_move;
    node_to_move->prev = NULL;
    dest->size = dest->size + 1;
}