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
** 1. sort_dispatch:
**   
	- Selects and executes the appropriate sorting algorithm based on stack size,
**      explicit strategy flags (1=SIMPLE, 2=MEDIUM, 3=COMPLEX),
	or calculated disorder metric.
**
** 2. clean_exit:
**   
	- Safely frees node memory and structure pointers for both Stack A and 
	Stack B, then returns the given exit code.  
**
** 3. main:
**    - Program entry point: parses arguments, builds and validates Stack A,
**      allocates Stack B, calculates disorder metric,
	executes sorting if unsorted,
**      prints benchmark metrics if requested, and cleans up resources on exit.
*/

	/* . build_stack handles parse_flags and fills Stack A */
	/* . ONLY allocate Stack B after A is fully validated (No R4 leaks!) */

static void	sort_dispatch(t_stack *a, t_stack *b)
{
	if (a->size == 2)
		sa(a);
	else if (a->size == 3)
		three_sorter(a);
	else if (a->size > 3)
	{
		if (a->strategy == 1)
			simple_sorter(a, b);
		else if (a->strategy == 2)
			medium_sorter(a, b);
		else if (a->strategy == 3)
			complex_sorter(a, b);
		else
		{
			if (a->disorder < 0.20)
				simple_sorter(a, b);
			else if (a->disorder >= 0.20 && a->disorder < 0.50)
				medium_sorter(a, b);
			else
				complex_sorter(a, b);
		}
	}
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
