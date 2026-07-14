# 04 — Metrics & Targeting (`disorder.c`, `algorithm_utils.c`)

Before sorting we **measure** the input (disorder) and, during sorting, we need to
**find where each element belongs** (targeting). Both are here. `algorithm_utils.c`
also holds `three_sorter`, the base case every strategy falls back to.

---

## A. `disorder.c` — how scrambled is the input?

### `compute_disorder` (lines 15–39)

```c
double compute_disorder(t_stack *stack)
{
    t_node *i, *j;
    double mistakes = 0.0;
    double total_pairs = 0.0;

    if (!stack || stack->size < 2)
        return (0.0);                    // 0 or 1 element is trivially sorted
    i = stack->head;
    while (i)
    {
        j = i->next;
        while (j)
        {
            total_pairs += 1.0;          // count this pair
            if (i->value > j->value)     // i is above j but larger → inversion
                mistakes += 1.0;
            j = j->next;
        }
        i = i->next;
    }
    return (mistakes / total_pairs);      // fraction in [0,1]
}
```

This is the **normalised inversion count** (a Kendall-tau-style measure). For every
ordered pair `(i, j)` where `i` sits above `j`, it's a "mistake" if `i`'s value is
larger (because we want ascending top-to-bottom). The result is
`mistakes / total_pairs`:

- `0.0` → already perfectly sorted.
- `~0.5` → random input (about half the pairs are inverted).
- near `1.0` → nearly reverse-sorted.

`total_pairs` for `n` elements is `n(n-1)/2`, so this loop is **O(n²)**. Acceptable
because it runs once, and n is at most a few thousand.

**Why we compute it:** `sort_dispatch` uses this single number to choose a
strategy. A nearly-sorted list (low disorder) sorts cheaply with the simple
insertion-style method; a fully random one needs the complex cost-based method.

> **Defense soundbite:** "Disorder is the fraction of value-pairs that are in the
> wrong order — 0 means sorted, ~0.5 means random. We measure it once, up front,
> and let it pick the algorithm."

---

## B. `algorithm_utils.c` — targeting + the 3-element base case

### `target_the_a` (lines 15–38) — where should a B-node land in A?

```c
t_node *target_the_a(t_node *node_b, t_stack *a)
{
    t_node *curr_a = a->head;
    t_node *target_node = NULL;
    long   closest_larger_value = 2147483648LL;  // just above INT_MAX

    while (curr_a)
    {
        if (curr_a->value > node_b->value
            && curr_a->value < closest_larger_value)  // smallest A-value still > b
        {
            closest_larger_value = curr_a->value;
            target_node = curr_a;
        }
        curr_a = curr_a->next;
    }
    if (target_node == NULL)     // node_b is larger than everything in A
        return (a->min);         // → it wraps on top of A's minimum
    return (target_node);
}
```

When we push a value back from B into A, A is kept **sorted ascending**. The
correct landing spot for `node_b` is the **smallest A-value that is still larger
than it** (so `node_b` slots in just below it). We scan A tracking that minimum.
If no A-value is larger (b is the new maximum), it belongs on top of A's `min`
(wrapping around a circular buffer) — that's the `return (a->min)`.

Init to `INT_MAX + 1` (as a `long`) guarantees the first qualifying candidate wins.

### `target_the_b` (lines 40–63) — where should an A-node land in B?

```c
t_node *target_the_b(t_node *node_a, t_stack *b)
{
    t_node *curr_b = b->head;
    t_node *target_node = NULL;
    long   closest_smaller_value = -2147483649LL;  // just below INT_MIN

    while (curr_b)
    {
        if (curr_b->value < node_a->value
            && curr_b->value > closest_smaller_value)  // largest B-value still < a
        {
            closest_smaller_value = curr_b->value;
            target_node = curr_b;
        }
        curr_b = curr_b->next;
    }
    if (target_node == NULL)     // node_a is smaller than everything in B
        return (b->max);         // → wraps on top of B's maximum
    return (target_node);
}
```

The mirror image. B is kept in **descending** order. When pushing an A-value into
B, its spot is the **largest B-value still smaller than it**. If none (a is the new
minimum), it wraps onto B's `max`. Used by the **simple** sorter while dumping A
into B.

> These two functions encode the invariants: **A ascending, B descending.** Keeping
> B descending is what makes pushing back to A cheap — the next value to return is
> always near B's top.

### `three_sorter` (lines 65–75) — sort exactly 3 elements

```c
void three_sorter(t_stack *stack)
{
    stack_o_meter(stack);                       // refresh pos/max
    if (stack->max->pos == 0)                    // max on top → send to bottom
        ra(stack);
    else if (stack->max->pos == 1)               // max in middle → send to bottom
        rra(stack);
    if (stack->head->value > stack->head->next->value)  // top two out of order
        sa(stack);
}
```

The universal base case — **every** strategy reduces to 3 elements and calls this.
Logic: first get the **maximum to the bottom** (position 2). If it's on top, one
`ra`; if in the middle, one `rra`. Once the max is at the bottom, only the top two
might be swapped — one `sa` fixes it. This sorts any of the 6 permutations of 3 in
**at most 2 operations**, provably optimal for n=3.

Walk the cases to prove it to an evaluator (values, positions top→bottom):
- `1 2 3` sorted → max at pos 2, top two ok → 0 ops.
- `1 3 2` → max at pos 1 → `rra` → `2 1 3`? no: `rra` brings bottom(2) to top →
  `2 1 3`; top two `2>1` → `sa` → `1 2 3`. (2 ops)
- `2 1 3` → max pos 2, top two `2>1` → `sa` → `1 2 3`. (1 op)
- `3 2 1` → max pos 0 → `ra` → `2 1 3`; `sa` → `1 2 3`. (2 ops)
- `3 1 2` → max pos 0 → `ra` → `1 2 3`. (1 op)
- `2 3 1` → max pos 1 → `rra` → `1 2 3`. (1 op)

All six sort correctly in ≤2 ops.

➡ Next: `05_algorithms.md`
