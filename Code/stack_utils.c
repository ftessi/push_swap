t_node	*node_creator(int value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = 0;
	node->pos = 0;
	node->median = false;
	node->cost = 0;
	node->disorder = 0;
	node->target_node = NULL;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}
void node_connecter (t_stack_check *stack, t_node *new_node)
{
    if (!stack || !new_node)
        return ;
    if (stack->size == 0)
    {
        stack->head = new_node;
        stack->tail = new_node;
    }
    else
    {
        stack->tail->next = new_node;
        new_node->prev = stack->tail;
        stack->tail = new_node;
    }
    stack->size++;
}


void    stack_o_meter(t_stack_check *stack)
{
    t_node *curr;
    int i;

    if (!stack || !stack->head)
        return ;
    //setting the values to start to fuck aroudn and find
    stack->max = stack->head;
    stack->min = stack->head;

    curr = stack->head;
    i = 0;

    while (curr)
    {
        curr->pos = i;
        curr->median = (i <= (stack->size / 2));//cuz median is bool <= this expression create true/false
        if (curr->value > stack->max->value)
            stack->max = curr;
        if (curr->value < stack->min->value)
            stack->min = curr;
        curr = curr->next;
        i++;
    }
}

/*t_node *max_finder(t_stack_node *stack)
{
    t_node *max;

    if (!stack || !stack->head)
        return (NULL);
    max = stack->head; 

    while (stack->head)
    {
        if (stack->head->value > max->value)
            max = stack->head;
        stack->head = stack->head->next;
    }
    return (max);
}
t_node *min_finder(t_stack_check *stack)
{
    t_node *min;

    if (!stack || !stack->head)
        return (NULL);
    min = stack->head;

    while (stack->head)
    {
        if (stack->head->value < min->value)
            min = stack->head;
        stack->head = stack->head->next;
    }
    return (min);
}*/
