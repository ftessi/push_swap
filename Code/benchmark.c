/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 13:25:15 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/16 13:45:20 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Func 1: Standard number printer adjusted for long to prevent overflows */
static void	ft_putnbr_fd(long n, int fd)
{
	char	c;

	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

/* Func 2: Rounds and prints a double to exactly 2 decimal places */
static void	ft_putdouble_fd(double d, int fd)
{
	long long	total_cents;
	long long	integer_part;
	long long	fractional_part;

	if (d < 0)
	{
		write(fd, "-", 1);
		d = -d;
	}
	total_cents = (long long)(d * 100.0 + 0.5);
	integer_part = total_cents / 100;
	fractional_part = total_cents % 100;
	ft_putnbr_fd(integer_part, fd);
	write(fd, ".", 1);
	if (fractional_part < 10)
		write(fd, "0", 1);
	ft_putnbr_fd(fractional_part, fd);
}

/* Func 3: Prints operational counts safely below 25 lines limit */
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

/* Func 4: Evaluates and prints complexity analysis dynamically */
static void	print_strategy_fd(t_stack *a, int fd)
{
	write(fd, "STRATEGY | ", 11);
	if (a->strategy == 1)
		write(fd, "Simple / O(n^2)\n", 16);
	else if (a->strategy == 2)
		write(fd, "Medium / O(n√n)\n", 18); 
	else if (a->strategy == 3)
		write(fd, "Complex / O(n log n)\n", 21);
	else
	{
		write(fd, "Adaptive (", 10);
		if (a->disorder < 0.20)
			write(fd, "Simple) / O(n^2)\n", 17);
		else if (a->disorder >= 0.20 && a->disorder < 0.50)
			write(fd, "Medium) / O(n√n)\n", 19);
		else
			write(fd, "Complex) / O(n log n)\n", 22);
	}
}

/* Func 5: Main entry point for the benchmarking summary dashboard */
void	print_benchmark_summary(t_stack *a, t_stack *b)
{
	int	total_ops;

	if (!a || !b)
		return ;
	total_ops = a->op_sa + b->op_sb + a->op_ss + a->op_pa + b->op_pb + a->op_ra
		+ b->op_rb + a->op_rr + a->op_rra + b->op_rrb + a->op_rrr;
	write(2, "\n ================== OPS SUMMARY ==================\n", 52);
	print_ops_details(a, b);
	write(2, "--------------------------------------------------\n", 51);
	write(2, "METRICS  | Initial Disorder: ", 29);
	ft_putdouble_fd(a->disorder, 2);
	write(2, "\n         | ", 12);
	print_strategy_fd(a, 2);
	write(2, "--------------------------------------------------\n", 51);
	write(2, " TOTAL MOVES: ", 14);
	ft_putnbr_fd(total_ops, 2);
	write(2, "\n==================================================\n\n", 53);
}