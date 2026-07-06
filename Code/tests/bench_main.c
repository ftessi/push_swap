/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:40:04 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/06 14:10:39 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../push_swap.h"

/* Helper function to display the detailed status of a stack */
void	print_stack_debug(t_stack_check *stack, char name)
{
	t_node	*current;

	printf("=== STACK %c STATUS ===\n", name);
	if (!stack || !stack->head)
	{
		printf("Stack %c is EMPTY.\n\n", name);
		return ;
	}
	printf("Size: %d\n", stack->size);
	printf("Min: %d | Max: %d\n", 
		stack->min ? stack->min->value : 0, 
		stack->max ? stack->max->value : 0);
	printf("Head: %d | Tail: %d\n", 
		stack->head->value, 
		stack->tail ? stack->tail->value : 0);
	printf("Elements (Forward) : ");
	current = stack->head;
	while (current)
	{
		printf("%d ", current->value);
		current = current->next;
	}
	printf("\nElements (Backward): ");
	current = stack->tail;
	while (current)
	{
		printf("%d ", current->value);
		current = current->prev;
	}
	printf("\n\n");
}

int	main(void)
{
	t_stack_check	*stack_a;
	t_stack_check	*stack_b;
	t_node			*new_node;

	stack_a = malloc(sizeof(t_stack_check));
	stack_b = malloc(sizeof(t_stack_check));
	if (!stack_a || !stack_b)
		return (1);
	stack_a->head = NULL; stack_a->tail = NULL; stack_a->max = NULL; stack_a->min = NULL; stack_a->size = 0;
	stack_b->head = NULL; stack_b->tail = NULL; stack_b->max = NULL; stack_b->min = NULL; stack_b->size = 0;

	printf("--- TEST 1: NODE CREATION AND CONNECTION ---\n");
	new_node = node_creator(24);
	node_connecter(stack_a, new_node);
	new_node = node_creator(-42);
	node_connecter(stack_a, new_node);
	new_node = node_creator(5);
	node_connecter(stack_a, new_node);

	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 2: SWAP (sa) OPERATION ---\n");
	printf("Executing sa...\n");
	sa(stack_a);
	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 3: PUSH (pb) OPERATION ---\n");
	printf("Executing pb (Moving top of A to B)...\n");
	pb(stack_a, stack_b);
	stack_o_meter(stack_a);
	stack_o_meter(stack_b);
	print_stack_debug(stack_a, 'A');
	print_stack_debug(stack_b, 'B');

	printf("--- TEST 4: ROTATE (ra) OPERATION ---\n");
	printf("Executing ra...\n");
	ra(stack_a);
	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 5: ALGORITHM (three_sorter) ---\n");
	printf("Bringing B's element back to A first...\n");
	pa(stack_a, stack_b);
	printf("Executing three_sorter on current unsorted sequence...\n");
	three_sorter(stack_a);
	stack_o_meter(stack_a);
	print_stack_debug(stack_a, 'A');

	printf("--- TEST 6: IS_SORTED CHECK ---\n");
	if (is_sorted(stack_a))
		printf("Stack A is fully sorted!\n");
	else
		printf("Stack A is NOT sorted yet.\n");

	free_stack(stack_a);
	free_stack(stack_b);
	free(stack_a);
	free(stack_b);
	printf("--- Test Finished, Memory Cleared ---\n");
	return (0);
}
