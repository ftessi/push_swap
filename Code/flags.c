/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 22:56:08 by ftessi            #+#    #+#             */
/*   Updated: 2026/07/28 22:56:28 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** FLAGS PARSER SUMMARY:
**
** 1. ft_strcmp:
**    - Compares two strings character by character and returns the difference
**      of the first non-matching unsigned characters.
**
** 2. is_flag_token:
**    - Checks if an argument starts with "--" to distinguish command flags
**      from negative numbers (e.g., -42 starts with a single '-').
**
** 3. apply_flag:
**    - Identifies recognized flags (--bench, --simple, --medium, --complex,
**      --adaptive) and sets the corresponding stack strategy/flag state.
**    - Returns true if applied successfully, or false if the flag is invalid.
**
** 4. parse_flags:
**    - Iterates over all command-line arguments to parse and apply flags.
**    - Counts non-flag (number) arguments and returns the total count,
**      or returns -1 if an invalid flag is detected.
*/

static int	ft_strcmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

int	is_flag_token(char *arg)
{
	return (arg[0] == '-' && arg[1] == '-');
}

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
