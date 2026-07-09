/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 00:51:13 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/09 14:33:05 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_pusher(t_stack_check *dest, t_stack_check *src)
{
	t_node	*node_to_move;

	if (!src->head)
		return ;
	node_to_move = src->head; // isolating the source head
	src->head = src->head->next;
	if (src->head)
		src->head->prev = NULL;
	else
		src->tail = NULL;
	src->size--;
	node_to_move->next = dest->head;
	if (dest->head)
		dest->head->prev = node_to_move;
	else
		dest->tail = node_to_move;
	dest->head = node_to_move;
	node_to_move->prev = NULL;
	dest->size++;
}
void	pb(t_stack_check *a, t_stack_check *b)
{
	stack_pusher(b, a); //(dest, src)
	b->op_pb++;
	write(1, "pb\n", 3);
}
void	pa(t_stack_check *a, t_stack_check *b)
{
	stack_pusher(a, b);
	a->op_pa++;
	write(1, "pa\n", 3);
}