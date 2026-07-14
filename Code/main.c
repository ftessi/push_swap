#include "push_swap.h"
#include <stdio.h>
#include <stdlib.h>

void	print_benchmark_summary(t_stack *a, t_stack *b)
{
	int	total_ops;

	if (!a || !b)
		return ;
	total_ops = a->op_sa + b->op_sb + a->op_ss + a->op_pa + b->op_pb + a->op_ra
		+ b->op_rb + a->op_rr + a->op_rra + b->op_rrb + a->op_rrr;
	printf("\n ================== OPS SUMMARY ==================\n");
	printf("SWAPS    | SA : %d\t| SB : %d\t| SS : %d\n", a->op_sa, b->op_sb,
		a->op_ss);
	printf("PUSHES   | PA : %d\t| PB : %d\n", a->op_pa, b->op_pb);
	printf("ROTATES  | RA : %d\t| RB : %d\t| RR : %d\n", a->op_ra, b->op_rb,
		a->op_rr);
	printf("REV-ROT  | RRA: %d\t| RRB: %d\t| RRR: %d\n", a->op_rra, b->op_rrb,
		a->op_rrr);
	printf("--------------------------------------------------\n");
	printf("METRICS  | Initial Disorder: %.2f\n", a->disorder);
	printf("--------------------------------------------------\n");
	printf(" TOTAL MOVES: %d\n", total_ops);
	printf("==================================================\n\n");
}

static void	sort_dispatch(t_stack *a, t_stack *b)
{
	if (a->size == 2)
		sa(a);
	else if (a->size == 3)
		three_sorter(a);
	else if (a->size > 3)
	{
		if (a->disorder < 0.20)
			simple_sorter(a, b);
		else if (a->disorder >= 0.20 && a->disorder < 0.50)
			medium_sorter(a, b);
		else
			complex_sorter(a, b);
	}
}

int	main(int argc, char **argv)
{
	t_stack *a;
	t_stack *b;

	if (argc < 2)
		return (0);
	a = init_stack();
		// init_stack içinde tüm op_ sayaçlarını 0 yaptığından emin ol!
	b = init_stack();
	if (!a || !b)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	build_stack(a, argc, argv);
	if (a->size > 0)
	{
		a->disorder = compute_disorder(a);
		if (!is_sorted(a))
			sort_dispatch(a, b);
	}
	if (a->bench)
		print_benchmark_summary(a, b);
	free_stack(a);
	free_stack(b);
	free(a);
	free(b);
	return (0);
}