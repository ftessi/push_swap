/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_swap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 03:29:48 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/29 21:53:04 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	stack_swapper(t_stack_check *stack)
{
	// lets approch our stack as A->B->C
	t_node *first;  // will be A
	t_node *second; // will be B
	if (!stack || stack->size < 2)
		return ;
	first = stack->head;  // head of the stack is A we put insdie of the first
	second = first->next; // next of the first is B we -put inside of the second
	// now we will change the nodes
	first->next = second->next;
		// second->next is C. we are connecting to A to C.
	first->prev = second;      
		// A is top thats why prev is NULL. We connected prec to second thats mean put B top of tha A.
	if (second->next)               // if C exits enter this loop
		second->next->prev = first; //  we connect C->prev node with A
	else                            // if there is only 2 element
		stack->tail = first;        // make the A the tail of the stack
	second->next = first;
		// first under the B was C. now B is top we need to connect A under B
	second->prev = NULL;  // B is top thats why it has NULL prev.
	stack->head = second; // head of the stack is B now.
}

void	sa(t_stack_check *a)
{
	stack_swapper(a);
	write(1, "sa\n", 3);
}

void	sb(t_stack_check *b)
{
	stack_swapper(b);
	write(1, "sb\n", 3);
}

void	ss(t_stack_check *a, t_stack_check *b)
{
	stack_swapper(a);
	stack_swapper(b);
	write(1, "ss\n", 3);
}
