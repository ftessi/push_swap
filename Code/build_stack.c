/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ukilicas <ukilicas@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:35:31 by ukilicas          #+#    #+#             */
/*   Updated: 2026/08/04 14:47:51 by ukilicas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** STACK BUILDING SUMMARY:
**
** 1. free_tokens:
**    - Frees a null-terminated array of string tokens allocated during split.
**
** 2. add_value:
**    - Validates syntax, converts string to integer, checks for duplicates,
**      creates a node, and appends it to stack A. Exits gracefully on error.
**
** 3. parse_split:
**    - Splits one argument on spaces, parses every token into stack A, and
**      frees the token array.
**
** 4. build_stack:
**    - Main initialization entry point: parses flags, then feeds every
**      non-flag argument through parse_split to populate stack A.
*/

static void	free_tokens(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	add_value(t_stack *a, char *tok, char **arr, bool is_split)
{
	int		value;
	t_node	*node;

	if (error_syntax(tok))
		free_and_exit(a, arr, is_split);
	value = (int)ft_atoi_ps(tok, a, arr, is_split);
	if (error_duplicate(a, value))
		free_and_exit(a, arr, is_split);
	node = node_creator(value);
	if (!node)
		free_and_exit(a, arr, is_split);
	node_connecter(a, node);
}

/*
** Every non-flag argument goes through the splitter, whether it holds one
** number or a whole quoted list. That makes "1 2 3", 1 2 3 and "1 2" "3"
** all behave identically. An argument that yields no token at all (an empty
** or blank string) is an error, which keeps ./push_swap "" 1 -> Error.
*/
static void	parse_split(t_stack *a, char *arg)
{
	char	**arr;
	int		i;

	arr = ft_split(arg, ' ');
	if (!arr)
		free_and_exit(a, NULL, false);
	if (!arr[0])
		free_and_exit(a, arr, true);
	i = 0;
	while (arr[i])
	{
		add_value(a, arr[i], arr, true);
		i++;
	}
	free_tokens(arr);
}

void	build_stack(t_stack *a, int argc, char **argv)
{
	int	i;
	int	nums;

	nums = parse_flags(a, argc, argv);
	if (nums < 0)
		free_and_exit(a, NULL, false);
	if (nums == 0)
		return ;
	i = 1;
	while (i < argc)
	{
		if (!is_flag_token(argv[i]))
			parse_split(a, argv[i]);
		i++;
	}
	if (a->size == 0)
		free_and_exit(a, NULL, false);
}
