# 03 — The Stack and the 11 Operations

This is the mechanical heart of the project: the linked-list plumbing
(`stack_utils.c`) and the 11 moves (`ops_*.c`). Every sorting strategy is just a
clever *sequence* of these moves. Each move has two layers:

- a **private mechanic** (`stack_pusher`, `stack_swapper`, `stack_rotater`,
  `stack_rev_rotater`) that rewires pointers, and
- **public wrappers** (`pa`, `sa`, `ra`, …) that call the mechanic, bump the
  matching counter, and `write` the op name to stdout.

The `write` to stdout is what the grader/checker reads. **Nothing else may go to
stdout** or the checker breaks.

---

## A. `stack_utils.c` — list lifecycle & queries

### `node_creator` (lines 15–31)

```c
t_node *node_creator(int value)
{
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->value = value;
    node->index = 0;  node->pos = 0;  node->median = false;  node->cost = 0;
    node->target = NULL;  node->next = NULL;  node->prev = NULL;
    return (node);
}
```

Allocates and **fully zero-initialises** one node, so strategy-specific fields
(`index`, `cost`, `target`) are safe to read before their strategy fills them.

### `node_connecter` (lines 32–48) — append to the tail

```c
void node_connecter(t_stack *stack, t_node *new_node)
{
    if (!stack || !new_node)
        return ;
    if (stack->size == 0)                 // first node: head == tail
    {
        stack->head = new_node;
        stack->tail = new_node;
    }
    else                                  // link after current tail
    {
        stack->tail->next = new_node;
        new_node->prev = stack->tail;
        stack->tail = new_node;
    }
    stack->size++;
}
```

Builds the stack in **input order**: `2 1 3` becomes head→2→1→3←tail. Appends at
the tail, keeping `head`, `tail`, `size` consistent. O(1) per insert thanks to the
cached `tail`.

### `stack_o_meter` (lines 50–74) — recompute metadata

```c
void stack_o_meter(t_stack *stack)
{
    t_node *curr;
    int    i;

    if (!stack || !stack->head)
        return ;
    stack->max = stack->head;             // seed extremes with the first node
    stack->min = stack->head;
    curr = stack->head;
    i = 0;
    while (curr)
    {
        curr->pos = i;                              // position from the top
        curr->median = (i <= (stack->size / 2));    // in the top half?
        if (curr->value > stack->max->value) stack->max = curr;
        if (curr->value < stack->min->value) stack->min = curr;
        curr = curr->next;
        i++;
    }
}
```

The "refresh" function. One pass that restamps every node's `pos`, recomputes
`median` (top-half flag used to pick `ra` vs `rra`), and re-finds `max`/`min`.
Strategies call this **after** they move things, because moves change positions.

Key idea — **`median` drives cheapest rotation direction**: if a target node is in
the top half (`pos <= size/2`), rotating *up* (`ra`) reaches it faster; otherwise
rotating *down* (`rra`) is shorter. That's how the simple/medium sorters avoid
wasteful rotations.

### `free_stack` (lines 75–92)

```c
void free_stack(t_stack *stack)
{
    t_node *curr, *next;
    if (!stack || !stack->head)
        return ;
    curr = stack->head;
    while (curr) { next = curr->next; free(curr); curr = next; }
    stack->head = NULL;  stack->tail = NULL;  stack->size = 0;
}
```

Frees every node (saving `next` before freeing `curr`, the classic list-free), then
resets the header fields. Note it frees the **nodes**, not the `t_stack` struct
itself — `main` does `free(a)` separately.

### `is_sorted` (lines 93–107)

```c
bool is_sorted(t_stack *stack)
{
    t_node *curr;
    if (!stack || !stack->head)
        return (true);            // empty/absent counts as sorted
    curr = stack->head;
    while (curr->next)
    {
        if (curr->value > curr->next->value)  // any descent → not sorted
            return (false);
        curr = curr->next;
    }
    return (true);
}
```

Defines "sorted" as **ascending from head (top) to tail (bottom)** — smallest on
top, which is the goal state. `main` uses it to skip sorting entirely if the input
is already sorted (so we emit zero ops).

---

## B. `ops_push.c` — `pa` / `pb`

### `stack_pusher` (lines 15–36) — move top of `src` onto top of `dest`

```c
void stack_pusher(t_stack *dest, t_stack *src)
{
    t_node *node_to_move;
    if (!src->head)
        return ;                       // nothing to push
    node_to_move = src->head;          // detach src's top
    src->head = src->head->next;
    if (src->head) src->head->prev = NULL;  // new src top has no prev
    else           src->tail = NULL;        // src became empty
    src->size--;
    node_to_move->next = dest->head;   // splice on top of dest
    if (dest->head) dest->head->prev = node_to_move;
    else            dest->tail = node_to_move;  // dest was empty
    dest->head = node_to_move;
    node_to_move->prev = NULL;
    dest->size++;
}
```

Detaches `src->head`, fixes `src`'s new head/tail/size, then inserts the node at
the front of `dest`. Every edge case (src becomes empty, dest was empty) is
handled. Pure pointer surgery — O(1).

### `pb` / `pa` (lines 37–48)

```c
void pb(t_stack *a, t_stack *b) { stack_pusher(b, a); b->op_pb++; write(1, "pb\n", 3); }
void pa(t_stack *a, t_stack *b) { stack_pusher(a, b); a->op_pa++; write(1, "pa\n", 3); }
```

`pb` pushes A→B (`dest=b, src=a`), counts on **B**, prints `pb`. `pa` is the
mirror. The counter lives on the destination stack.

---

## C. `ops_swap.c` — `sa` / `sb` / `ss`

### `stack_swapper` (lines 15–37) — swap the top two nodes

```c
void stack_swapper(t_stack *stack)
{
    t_node *first, *second;
    if (!stack || stack->size < 2)     // need at least 2 to swap
        return ;
    first = stack->head;               // A
    second = first->next;              // B  (we want B on top)
    first->next = second->next;        // A now points to C
    first->prev = second;              // A sits under B
    if (second->next)                  // if C exists, fix its prev
        second->next->prev = first;
    else                               // only 2 elements → A is new tail
        stack->tail = first;
    second->next = first;              // B points down to A
    second->prev = NULL;               // B is the new top
    stack->head = second;
}
```

Swaps the first two nodes by relinking four pointers. The `if (second->next)`
branch handles the 2-element case (A becomes the tail). O(1). *(This function has
the most inline comments in the codebase — remember to strip them for norminette.)*

### `sa` / `sb` / `ss` (lines 39–59)

```c
void sa(t_stack *a)            { stack_swapper(a); a->op_sa++; write(1, "sa\n", 3); }
void sb(t_stack *b)            { stack_swapper(b); b->op_sb++; write(1, "sb\n", 3); }
void ss(t_stack *a, t_stack *b){ stack_swapper(a); stack_swapper(b); a->op_ss++; write(1, "ss\n", 3); }
```

`ss` swaps both stacks and counts as **one** op (`op_ss`), consistent with the
cost model. Our sorters don't actually emit `ss`/`sb` much, but the ops exist for
completeness and the checker.

---

## D. `ops_rotate.c` — `ra` / `rb` / `rr` (rotate UP: top → bottom)

### `stack_rotater` (lines 15–33)

```c
void stack_rotater(t_stack *stack)
{
    t_node *old_head, *new_head;
    if (!stack || stack->size < 2)
        return ;
    old_head = stack->head;
    new_head = old_head->next;
    stack->head = new_head;          // 2nd element becomes the top
    new_head->prev = NULL;
    old_head->next = NULL;           // old top goes to the bottom
    old_head->prev = stack->tail;
    stack->tail->next = old_head;
    stack->tail = old_head;
}
```

Rotate up: the top element wraps around to become the new **bottom**. Everything
shifts up by one. O(1) via cached `tail`.

### `ra` / `rb` / `rr` (lines 35–54)

```c
void ra(t_stack *a)             { stack_rotater(a); a->op_ra++; write(1, "ra\n", 3); }
void rb(t_stack *b)             { stack_rotater(b); b->op_rb++; write(1, "rb\n", 3); }
void rr(t_stack *a, t_stack *b) { stack_rotater(a); stack_rotater(b); a->op_rr++; write(1, "rr\n", 3); }
```

`rr` rotates both, counts once. `rr`/`rrr` are the efficiency trick the complex
sorter exploits: rotating A and B **simultaneously** costs one op instead of two.

---

## E. `ops_rev_rotate.c` — `rra` / `rrb` / `rrr` (rotate DOWN: bottom → top)

### `stack_rev_rotater` (lines 15–32)

```c
void stack_rev_rotater(t_stack *stack)
{
    t_node *old_tail, *new_tail;
    if (!stack || stack->size < 2)
        return ;
    old_tail = stack->tail;
    new_tail = old_tail->prev;
    stack->tail = new_tail;          // 2nd-from-bottom becomes the bottom
    new_tail->next = NULL;
    old_tail->prev = NULL;           // old bottom goes to the top
    old_tail->next = stack->head;
    stack->head->prev = old_tail;
    stack->head = old_tail;
}
```

The exact inverse of `stack_rotater`: the bottom element wraps to the **top**.
This is why we needed `prev` pointers — detaching the tail is O(1).

### `rra` / `rrb` / `rrr` (lines 34–53)

```c
void rra(t_stack *a)             { stack_rev_rotater(a); a->op_rra++; write(1, "rra\n", 4); }
void rrb(t_stack *b)             { stack_rev_rotater(b); b->op_rrb++; write(1, "rrb\n", 4); }
void rrr(t_stack *a, t_stack *b) { stack_rev_rotater(a); stack_rev_rotater(b); a->op_rrr++; write(1, "rrr\n", 4); }
```

Note the length is **4** here (`"rra\n"`), vs 3 for the two-letter ops — an easy
off-by-one to get wrong; it's correct in the code.

---

## Cross-cutting points for defense

- **Every op is O(1).** No searching inside a move — the strategies do the
  thinking, the moves just relink pointers.
- **`head`/`tail`/`size` are always kept consistent** by the four mechanics, so any
  strategy can trust them and `stack_o_meter` right after.
- **stdout discipline:** the *only* thing written to fd 1 is the op stream. That's
  what makes `./push_swap $ARG | ./checker $ARG` work.
- **Counters** are incremented in the wrappers, not the mechanics, so an internal
  call (e.g. `stack_swapper` used by a future combined op) wouldn't double-count.

➡ Next: `04_metrics_disorder_targets.md`
