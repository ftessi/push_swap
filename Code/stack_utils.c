/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukilicas <ukilicas@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 21:34:06 by ukilicas          #+#    #+#             */
/*   Updated: 2026/08/04 14:49:24 by ukilicas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** STACK UTILS SUMMARY:
**
** 1. node_creator:
**    - Allocates memory for a new node and initializes default attributes
**      (value, index, pos, median, cost, target, next, prev).
**
** 2. node_connecter:
**    - Appends a new node to the tail of the stack and updates pointers
**      along with the stack size.
**
** 3. stack_o_meter:
**   
	- Updates position indices and median status (top half = true) for all nodes,
**      while tracking the minimum and maximum node pointers in the stack.
**
** 4. free_stack:
**    - Frees all allocated node memory in the stack and resets stack head,
**      tail, and size.
**
** 5. is_sorted:
**    - Scans the stack to verify if elements are sorted in ascending order.
**      Returns true if sorted, false otherwise.
*/

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
	stack->max = stack->head;
	stack->min = stack->head;
	curr = stack->head;
	i = 0;
	while (curr)
	{
		curr->pos = i;
		curr->median = (i <= (stack->size / 2));
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
			return (false);
		curr = curr->next;
	}
	return (true);
}
