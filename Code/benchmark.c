/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 13:25:15 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/08/04 00:45:02 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** BENCHMARK SUMMARY (--bench, stderr only):
**
** 1. print_ops_details:
**    - Prints the count of all 11 operation types. Dual operations (ss, rr,
**      rrr) are counted once, on stack A, because they are one instruction.
**
** 2. print_class_fd:
**    - Prints one strategy name and its complexity class in the push_swap
**      operation model. TINY and NONE only ever come from a->resolved.
**
** 3. print_strategy_fd:
**    - SELECTED is what the flags plus the disorder asked for; it always
**      resolves through resolve_strategy(), the same function main.c uses
**      to dispatch, so the two can never drift apart.
**    - EXECUTED is what actually generated the operations. The two differ
**      when the stack was already sorted (NONE) or had 2-3 elements (TINY).
*/

static void	print_ops_details(t_stack *a, t_stack *b)
{
	write(2, "SWAPS    | SA : ", 16);
	ft_putnbr_fd(a->op_sa, 2);
	write(2, "\t| SB : ", 8);
	ft_putnbr_fd(b->op_sb, 2);
	write(2, "\t| SS : ", 8);
	ft_putnbr_fd(a->op_ss, 2);
	write(2, "\nPUSHES   | PA : ", 17);
	ft_putnbr_fd(a->op_pa, 2);
	write(2, "\t| PB : ", 8);
	ft_putnbr_fd(b->op_pb, 2);
	write(2, "\nROTATES  | RA : ", 17);
	ft_putnbr_fd(a->op_ra, 2);
	write(2, "\t| RB : ", 8);
	ft_putnbr_fd(b->op_rb, 2);
	write(2, "\t| RR : ", 8);
	ft_putnbr_fd(a->op_rr, 2);
	write(2, "\nREV-ROT  | RRA: ", 17);
	ft_putnbr_fd(a->op_rra, 2);
	write(2, "\t| RRB: ", 8);
	ft_putnbr_fd(b->op_rrb, 2);
	write(2, "\t| RRR: ", 8);
	ft_putnbr_fd(a->op_rrr, 2);
	write(2, "\n", 1);
}

static void	print_class_fd(t_strategy s, int fd)
{
	if (s == SIMPLE)
		write(fd, "Simple / O(n^2)", 15);
	else if (s == MEDIUM)
		write(fd, "Medium / O(n*sqrt(n))", 21);
	else if (s == COMPLEX)
		write(fd, "Complex / O(n log n)", 20);
	else if (s == TINY)
		write(fd, "Direct micro-sort / O(1)", 24);
	else
		write(fd, "None (input already sorted)", 27);
}

static void	print_strategy_fd(t_stack *a, int fd)
{
	write(fd, "SELECTED : ", 11);
	if (a->strategy == ADAPTIVE)
		write(fd, "Adaptive -> ", 12);
	print_class_fd(resolve_strategy(a), fd);
	write(fd, "\n         | EXECUTED : ", 23);
	print_class_fd(a->resolved, fd);
	write(fd, "\n", 1);
}

int	total_operations(t_stack *a, t_stack *b)
{
	return (a->op_sa + b->op_sb + a->op_ss + a->op_pa + b->op_pb + a->op_ra
		+ b->op_rb + a->op_rr + a->op_rra + b->op_rrb + a->op_rrr);
}

void	print_benchmark_summary(t_stack *a, t_stack *b)
{
	if (!a || !b)
		return ;
	write(2, "\n ================== OPS SUMMARY ==================\n", 52);
	print_ops_details(a, b);
	write(2, "--------------------------------------------------\n", 51);
	write(2, "METRICS  | Initial Disorder: ", 29);
	ft_putpercent_fd(a->disorder, 2);
	write(2, "\n         | ", 12);
	print_strategy_fd(a, 2);
	write(2, "--------------------------------------------------\n", 51);
	write(2, " TOTAL MOVES: ", 14);
	ft_putnbr_fd(total_operations(a, b), 2);
	write(2, "\n==================================================\n\n", 53);
}
