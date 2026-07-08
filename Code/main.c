#include "push_swap.h"
#include <stdio.h>
#include <stdlib.h>

static t_stack_check	*init_stack(void)
{
	t_stack_check	*stack;

	stack = malloc(sizeof(t_stack_check));
	if (!stack)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->max = NULL;
	stack->min = NULL;
	stack->size = 0;
	stack->disorder = 0.0;
	return (stack);
}

static void	sort_dispatch(t_stack_check *a, t_stack_check *b)
{
	if (a->size == 2)
		sa(a);
// Just changing the order because it's already not sorted and only 2
	else if (a->size == 3)
		three_sorter(a);
	else if (a->size > 3)
		simple_sorter(a, b);
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
	printf("\n Disorder stack: %f", a->disorder);
	printf("\n Hi!");
	printf("\n Hi!");
	if (!is_sorted(a))
		sort_dispatch(a, b);
	free_stack(a);
	free_stack(b);
	free(a);
	free(b);
	return (0);
}
