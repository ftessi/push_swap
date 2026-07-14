/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 17:55:55 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/14 16:05:58 by ftessi           ###   ########.fr       */
/*                                                                            */
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

/* a flag is exactly a token starting with "--"; a negative number
   like -42 starts with a single '-', so it is NOT caught here */

int	is_flag_token(char *arg)
{
	return (arg[0] == '-' && arg[1] == '-');
}

/* set the state a recognised flag, if not part of the recognized flags,
	error */
static void	apply_flag(t_stack *a, char *arg)
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
		free_and_exit(a, NULL, false);
}

/* single pass: apply every flag, return how many NUMBER of args exist */
int	parse_flags(t_stack *a, int argc, char **argv)
{
	int	i;
	int	nums;

	i = 1;
	nums = 0;
	while (i < argc)
	{
		if (is_flag_token(argv[i]))
			apply_flag(a, argv[i]);
		else
			nums++;
		i++;
	}
	return (nums);
}

/* return the first non-falg argument (applied also for quoted string case) */
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
