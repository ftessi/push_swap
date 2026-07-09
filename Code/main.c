#include "push_swap.h"
#include <stdio.h>
#include <stdlib.h>

/*static int	ft_strcmp(char *s1, char *s2)
{
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;
	int	total_ops;

	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}*/
void	print_benchmark_summary(t_stack_check *a, t_stack_check *b)
{
	total_ops = a->op_sa + b->op_sa + a->op_pa + b->op_pa + a->op_pb + b->op_pb
		+ a->op_ra + b->op_ra + a->op_rb + b->op_rb + a->op_rr + b->op_rr
		+ a->op_rra + b->op_rra + a->op_rrb + b->op_rrb + a->op_rrr + b->op_rrr;
	printf("\n === OPS SUMMERY === \n");
	printf("SA : %d\n", a->op_sa);
	printf("PA : %d | PB : %d\n", a->op_pa, b->op_pb);
	printf("RA : %d | RB : %d | RR : %d\n", a->op_ra, b->op_rb, a->op_rr);
	printf("RRA: %d | RRB: %d | RRR: %d\n", a->op_rra, b->op_rrb, a->op_rrr);
	printf("----------------------------------\n");
	printf("TOTAL MOVES: %d\n", total_ops);
	printf("==================================\n\n");
}

static void	sort_dispatch(t_stack_check *a, t_stack_check *b)
{
	if (a->size == 2)
		sa(a);
	// Just changing the order because it's already not sorted and only 2
	else if (a->size == 3)
		three_sorter(a);
	else if (a->size > 3)
	{
		complex_sorter(a, b);
	}
	// The three sorter magic
	// Now the algo's
}

int	main(int argc, char **argv)
{
	t_stack_check	*a;
	t_stack_check	*b;

	if (argc < 2)
		return (0);
	a = init_stack();
	b = init_stack();
	if (!a || !b)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	build_stack(a, argc, argv);
	a->disorder = compute_disorder(a);
	if (!is_sorted(a))
	{
		sort_dispatch(a, b);
	}
	free_stack(a);
	free_stack(b);
	free(a);
	free(b);
	return (0);
}
