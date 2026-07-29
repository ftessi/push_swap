/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftessi <ftessi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:47:29 by umutkilicas       #+#    #+#             */
/*   Updated: 2026/07/28 22:55:34 by ftessi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** MAIN PROGRAM & DISPATCH SUMMARY:
**
** 1. resolve_strategy:
**    - Turns the *selected* strategy into the concrete one that will run.
**    - A forced flag is returned as-is; ADAPTIVE is resolved against the
**      disorder measured before any move, using the subject's thresholds
**      (< 0.20 -> O(n^2), < 0.50 -> O(n*sqrt(n)), else O(n log n)).
**    - This is the ONLY place the thresholds live: benchmark.c calls this
**      same function, so the two can never disagree.
**
** 2. sort_dispatch:
**    - Records in a->resolved the algorithm that actually generates the
**      operations, then runs it. Stacks of 2 or 3 use a hardcoded micro-sort
**      (TINY) because no general strategy can beat 1-2 operations there.
**
** 3. clean_exit:
**    - Frees node memory and the structures themselves for both stacks,
**      then returns the given exit code.
**
** 4. main:
**    - Parses arguments, builds and validates Stack A, allocates Stack B,
**      measures disorder, sorts if needed, prints benchmark metrics if
**      requested, and cleans up on exit.
*/

	/* . build_stack handles parse_flags and fills Stack A */
	/* . ONLY allocate Stack B after A is fully validated (No R4 leaks!) */

t_strategy	resolve_strategy(t_stack *a)
{
	if (a->strategy != ADAPTIVE)
		return (a->strategy);
	if (a->disorder < 0.20)
		return (SIMPLE);
	if (a->disorder < 0.50)
		return (MEDIUM);
	return (COMPLEX);
}

static void	sort_dispatch(t_stack *a, t_stack *b)
{
	a->resolved = resolve_strategy(a);
	if (a->size == 2)
	{
		a->resolved = TINY;
		sa(a);
	}
	else if (a->size == 3)
	{
		a->resolved = TINY;
		three_sorter(a);
	}
	else if (a->resolved == SIMPLE)
		simple_sorter(a, b);
	else if (a->resolved == MEDIUM)
		medium_sorter(a, b);
	else
		complex_sorter(a, b);
}

static int	clean_exit(t_stack *a, t_stack *b, int exit_code)
{
	if (a)
	{
		free_stack(a);
		free(a);
	}
	if (b)
	{
		free_stack(b);
		free(b);
	}
	return (exit_code);
}

int	main(int argc, char **argv)
{
	t_stack	*a;
	t_stack	*b;

	if (argc < 2)
		return (0);
	a = init_stack();
	if (!a)
		return (write(2, "Error\n", 6), 1);
	build_stack(a, argc, argv);
	if (a->size == 0)
		return (clean_exit(a, NULL, 0));
	b = init_stack();
	if (!b)
	{
		write(2, "Error\n", 6);
		return (clean_exit(a, NULL, 1));
	}
	a->disorder = compute_disorder(a);
	if (!is_sorted(a))
		sort_dispatch(a, b);
	if (a->bench)
		print_benchmark_summary(a, b);
	return (clean_exit(a, b, 0));
}
