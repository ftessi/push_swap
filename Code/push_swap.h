/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:33:03 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/06/27 03:06:45 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_node		t_node;

typedef struct s_node
{
	int value;    // the actual value of the input
	int index;    // index value of the input on right order
	int pos;      // current positon on the stack
	bool median;  // if 1 above median if 0 under
	int cost;     // total cost of ops
	int disorder; //
	struct t_node			*target_node;
	struct t_node			*next;
	struct t_node			*prev;
};

typedef struct s_stack_check
{
	struct s_stack_check	*head;
	struct s_stack_check	*tail;
	int						disorder;
	int						size;
}   t_stack_check;