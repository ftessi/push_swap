/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* flags.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: umutkilicaslan <umutkilicaslan@student.    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/07/10 17:55:55 by ftessi            #+#    #+#             */
/* Updated: 2026/07/15 15:30:00 by umutkilicas      ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_strcmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/* A flag is exactly a token starting with "--"; a negative number
   like -42 starts with a single '-', so it is NOT caught here */
int	is_flag_token(char *arg)
{
	return (arg[0] == '-' && arg[1] == '-');
}

/* Set the state of a recognised flag. Return false if the flag is invalid */
static bool	apply_flag(t_stack *a, char *arg)
{
	if (!ft_strcmp(arg, "--bench"))
		a->bench = true;
	else if (!ft_strcmp(arg, "--simple"))
		a->strategy = SIMPLE;
	else if (!ft_strcmp(arg, "--medium"))
		a->strategy = MEDIUM;
	else if (!ft_strcmp(arg, "--complex"))
		a->strategy = COMPLEX;
	else if (!ft_strcmp(arg, "--adaptive"))
		a->strategy = ADAPTIVE;
	else
		return (false);
	return (true);
}

/* Single pass: apply every flag, return how many NUMBER of args exist.
   Returns -1 if an invalid flag is detected. */
int	parse_flags(t_stack *a, int argc, char **argv)
{
	int	i;
	int	nums;

	i = 1;
	nums = 0;
	while (i < argc)
	{
		if (is_flag_token(argv[i]))
		{
			if (!apply_flag(a, argv[i]))
				return (-1);
		}
		else
			nums++;
		i++;
	}
	return (nums);
}

/* Return the first non-flag argument (applied also for quoted string case) */
char	*first_number(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_flag_token(argv[i]))
			return (argv[i]);
		i++;
	}
	return (NULL);
}