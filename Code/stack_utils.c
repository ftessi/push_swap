int stack_size (t_stack_node *stack)
{
    int count;

    if (!stack)
        return (0);
    count = 0;
    while (stack)
    {
        count++;
        stack = stack->next;
    }
    return (count);
}

t_stack_node    *find_last(t_stack_node *stack)
{
    if(!stack)
        return (NULL);
    while (stack->next)
        stack = stack->next;
    return (stack);
}
t_stack_node *find_max(t_stack_node *stack)
{
    long                  max;
    t_stack_node    *max_node;

    if (!stack)
        return (NULL);
}