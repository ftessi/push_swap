/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:42:49 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/15 15:42:55 by umutkilicas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

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

/* Rounds and prints a double to exactly 2 decimal places using fd 2 */
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

void	print_benchmark_summary(t_stack *a, t_stack *b)
{
	int	total_ops;

	if (!a || !b)
		return ;
	total_ops = a->op_sa + b->op_sb + a->op_ss + a->op_pa + b->op_pb + a->op_ra
		+ b->op_rb + a->op_rr + a->op_rra + b->op_rrb + a->op_rrr;
	ft_putstr_fd("\n ================== OPS SUMMARY ==================\n", 2);
	ft_putstr_fd("SWAPS    | SA : ", 2);
	ft_putnbr_fd(a->op_sa, 2);
	ft_putstr_fd("\t| SB : ", 2);
	ft_putnbr_fd(b->op_sb, 2);
	ft_putstr_fd("\t| SS : ", 2);
	ft_putnbr_fd(a->op_ss, 2);
	ft_putstr_fd("\nPUSHES   | PA : ", 2);
	ft_putnbr_fd(a->op_pa, 2);
	ft_putstr_fd("\t| PB : ", 2);
	ft_putnbr_fd(b->op_pb, 2);
	ft_putstr_fd("\nROTATES  | RA : ", 2);
	ft_putnbr_fd(a->op_ra, 2);
	ft_putstr_fd("\t| RB : ", 2);
	ft_putnbr_fd(b->op_rb, 2);
	ft_putstr_fd("\t| RR : ", 2);
	ft_putnbr_fd(a->op_rr, 2);
	ft_putstr_fd("\nREV-ROT  | RRA: ", 2);
	ft_putnbr_fd(a->op_rra, 2);
	ft_putstr_fd("\t| RRB: ", 2);
	ft_putnbr_fd(b->op_rrb, 2);
	ft_putstr_fd("\t| RRR: ", 2);
	ft_putnbr_fd(a->op_rrr, 2);
	ft_putstr_fd("\n--------------------------------------------------\n", 2);
	ft_putstr_fd("METRICS  | Initial Disorder: ", 2);
	ft_putdouble_fd(a->disorder, 2);
	ft_putstr_fd("\n--------------------------------------------------\n", 2);
	ft_putstr_fd(" TOTAL MOVES: ", 2);
	ft_putnbr_fd(total_ops, 2);
	ft_putstr_fd("\n==================================================\n\n", 2);
}