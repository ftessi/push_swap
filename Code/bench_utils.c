/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 10:12:04 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/29 10:12:04 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** BENCHMARK OUTPUT HELPERS:
**
** 1. ft_putnbr_fd:
**    - Writes a signed long to a file descriptor, digit by digit.
**
** 2. ft_putpercent_fd:
**    - Takes the disorder as a ratio in [0, 1] and writes it as a percentage
**      with exactly two decimals plus the '%' sign, as the subject requires
**      ("The computed disorder (% with two decimals)").
**    - Scales to hundredths of a percent first, then rounds once, so a
**      disorder of 0.003 prints "0.30%" instead of collapsing to "0.00".
*/

void	ft_putnbr_fd(long n, int fd)
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

void	ft_putpercent_fd(double ratio, int fd)
{
	long	hundredths;
	long	whole;
	long	frac;

	if (ratio < 0)
	{
		write(fd, "-", 1);
		ratio = -ratio;
	}
	hundredths = (long)(ratio * 10000.0 + 0.5);
	whole = hundredths / 100;
	frac = hundredths % 100;
	ft_putnbr_fd(whole, fd);
	write(fd, ".", 1);
	if (frac < 10)
		write(fd, "0", 1);
	ft_putnbr_fd(frac, fd);
	write(fd, "%", 1);
}
