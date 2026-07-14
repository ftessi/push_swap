/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 21:34:06 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/14 16:08:26 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*node_creator(int value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = 0;
	node->pos = 0;
	node->median = false;
	node->cost = 0;
	node->target = NULL;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}
void	node_connecter(t_stack *stack, t_node *new_node)
{
	if (!stack || !new_node)
		return ;
	if (stack->size == 0)
	{
		stack->head = new_node;
		stack->tail = new_node;
	}
	else
	{
		stack->tail->next = new_node;
		new_node->prev = stack->tail;
		stack->tail = new_node;
	}
	stack->size++;
}

void	stack_o_meter(t_stack *stack)
{
	t_node	*curr;
	int		i;

	if (!stack || !stack->head)
		return ;
	// setting the values to start to fuck aroudn and find
	stack->max = stack->head;
	stack->min = stack->head;
	curr = stack->head;
	i = 0;
	while (curr)
	{
		curr->pos = i;
		curr->median = (i <= (stack->size / 2));
			// cuz median is bool <= this expression create true/false
		if (curr->value > stack->max->value)
			stack->max = curr;
		if (curr->value < stack->min->value)
			stack->min = curr;
		curr = curr->next;
		i++;
	}
}
void	free_stack(t_stack *stack)
{
	t_node	*curr;
	t_node	*next;

	if (!stack || !stack->head)
		return ;
	curr = stack->head;
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
}
bool	is_sorted(t_stack *stack)
{
	t_node	*curr;

	if (!stack || !stack->head)
		return (true);
	curr = stack->head;
	while (curr->next)
	{
		if (curr->value > curr->next->value)
			return (false); // cuz desencing order
		curr = curr->next;
	}
	return (true);
}
