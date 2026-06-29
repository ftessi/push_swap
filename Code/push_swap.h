/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:33:03 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/28 21:33:59 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_stack_node
{
	int						value;
	// the actual value of the input
	int						index;
	// index value of the input on right order
	int						pos;
	// current positon on the stack
	bool					median;
	// if 1 above median if 0 under
	int						cost;
	// total cost of ops
	double					disorder;
	// from 0 to 1 and measures how far
	// away is from being completely ordered
	t_stack_node			*target;
	t_stack_node			*next;
	t_stack_node			*prev;
}	t_stack_node;

typedef struct s_stack_check
{
	struct s_stack_check	*head;
	struct s_stack_check	*tail;
	struct s_stack_check	*max;
	struct s_stack_check	*min;
	int						disorder;
	// again disorder?
	int						size;
	// how many nodes
}							t_stack_check;

#endif