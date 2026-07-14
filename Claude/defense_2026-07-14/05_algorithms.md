# 05 — The Sorting Algorithms

Three strategies, each in its own file, all reducing to `three_sorter` for the last
3 elements. They share the same skeleton — *push most of A into B, sort the last 3,
then push B back into A in the right order* — but differ in **how cleverly** they
choose moves.

| Strategy | File | Idea | Rough cost |
|----------|------|------|-----------|
| Simple  | `simple.c`  | Insertion via nearest-target in B | O(n²)-ish |
| Medium  | `medium.c`  | Chunk/bucket by rank | O(n√n)-ish |
| Complex | `complex.c` + `complex_utils.c` | Cheapest-cost greedy (Turk-style) | O(n log n)-ish |

Selection happens in `main.c`'s `sort_dispatch` by **disorder** (see doc `06`).

---

## A. `simple.c` — `simple_sorter` (lines 16–78)

Insertion-style sort. Keeps **B descending** and inserts each pushed element at its
correct spot using `target_the_b`.

### Phase 1 — dump A into B, keeping B ordered (lines 22–42)

```c
while (a->size > 3)
{
    stack_o_meter(a);
    stack_o_meter(b);
    target_node = target_the_b(a->head, b);   // where a->head fits in B
    while (b->head != target_node)             // rotate B so that slot is on top
    {
        if (target_node->median == true)
            rb(b);                             // slot in top half → rotate up
        else
            rrb(b);                            // slot in bottom half → rotate down
    }
    pb(a, b);                                  // push a->head onto that slot
}
```

For each element on top of A, find its landing slot in B, rotate B (choosing the
**shorter** direction via `median`) until that slot is on top, then push. B stays
sorted the whole time. We stop when A has 3 left.

### Phase 2 — sort the last 3 (line 45)

```c
three_sorter(a);
```

### Phase 3 — push B back into A in ascending order (lines 48–65)

```c
while (b->size > 0)
{
    stack_o_meter(a);
    stack_o_meter(b);
    target_node = target_the_a(b->head, a);   // where b->head fits in A
    while (a->head != target_node)             // rotate A to expose that slot
    {
        if (target_node->median == true) ra(a);
        else                              rra(a);
    }
    pa(a, b);                                  // push it home
}
```

Symmetric to phase 1. Because B is descending, `b->head` is always the next value
to place, and `target_the_a` finds its ascending slot in A.

### Phase 4 — final alignment (lines 68–77)

```c
stack_o_meter(a);
while (a->head != a->min)          // rotate smallest element to the top
{
    if (a->min->median == true) ra(a);
    else                        rra(a);
}
```

After phase 3, A is sorted but maybe **rotated** (min not on top). Rotate the
minimum to the top, in the cheaper direction, to finish. Note `simple.c` includes
`<stdio.h>` but doesn't use it — a leftover include to remove (see `07`).

---

## B. `medium.c` — `medium_sorter` (lines 39–101)

Bucket/chunk sort. Ranks every element `0..n-1`, then pushes A into B in **chunks**,
which keeps B roughly ordered without per-element target searches.

### `assign_rank` (lines 15–37) — normalise values to ranks

```c
static void assign_rank(t_stack *stack)
{
    t_node *curr = stack->head;
    while (curr)
    {
        int rank = 0;
        t_node *compr = stack->head;
        while (compr)
        {
            if (curr->value > compr->value) rank++;  // count smaller values
            compr = compr->next;
        }
        curr->index = rank;      // 0 = smallest ... n-1 = largest
        curr = curr->next;
    }
}
```

O(n²) ranking that maps arbitrary ints onto `0..n-1` in `node->index`, so chunk
boundaries can be simple integer comparisons regardless of the raw values.

### Chunk distribution (lines 41–69)

```c
if (a->size <= 100) chunk_size = 18;   // tuned for n≈100
else                chunk_size = 45;   // tuned for n≈500
idx = 0;
while (a->size > 3)
{
    if (a->head->index <= idx)         // in the chunk already fully placed
    {
        pb(a, b);
        rb(b);                          // shove it to the bottom of B
        idx++;
    }
    else if (a->head->index <= (idx + chunk_size))  // in the current window
    {
        pb(a, b);                       // keep it near the top of B
        idx++;
    }
    else
        ra(a);                          // not in this chunk yet → skip it for now
}
```

We walk A's top: if the element belongs to an already-consumed chunk we push+bury
it (`rb`), if it's in the current chunk window we just push it, otherwise we rotate
it away to look at it on a later pass. Pushing in rank-order chunks means B ends up
**approximately** sorted, cutting the work in phase 3. `chunk_size` is tuned
(18 / 45) to keep total ops low for the two grader sizes.

### Phases 3 & 4 (lines 71–101)

```c
three_sorter(a);                          // base case
while (b->size > 0)                       // push back, exact ascending target
{
    stack_o_meter(a); stack_o_meter(b);
    target_node = target_the_a(b->head, a);
    while (target_node->pos != 0)         // rotate A to expose the slot
    {
        if (target_node->median == true) ra(a);
        else                              rra(a);
        stack_o_meter(a);                 // re-measure after each rotate
    }
    pa(a, b);
}
stack_o_meter(a);
while (a->min->pos != 0)                   // final: min to the top
{
    if (a->min->median == true) ra(a);
    else                        rra(a);
    stack_o_meter(a);
}
```

Same "push home + align" tail as simple, but it re-runs `stack_o_meter` **inside**
the rotation loop (positions change every rotate). Correct, though a touch more
expensive than needed.

---

## C. `complex.c` + `complex_utils.c` — the Turk (cheapest-cost) sort

The best strategy for high disorder. Instead of a fixed policy, it **prices every
possible move** and always does the cheapest one. This is the "radix/Turk" family.

### `complex_utils.c` support functions

**`signed_rot` (lines 15–20)** — how many rotations, with direction, to bring
position `pos` to the top:

```c
int signed_rot(int pos, int size)
{
    if (pos * 2 <= size)    // in the top half
        return (pos);       // positive = rotate up (ra/rb), `pos` times
    return (pos - size);    // negative = rotate down (rra/rrb), |pos-size| times
}
```

A single number that encodes both **count** and **direction**: positive → rotate
up; negative → reverse-rotate. This is what lets the cost model add A- and B-costs
and detect when they share a direction (→ combine into `rr`/`rrr`).

**`abs_i` (lines 22–27)** — integer absolute value helper.

**`assign_cost` (lines 29–51)** — price every B-node:

```c
void assign_cost(t_stack *a, t_stack *b)
{
    stack_o_meter(a); stack_o_meter(b);
    curr = b->head;
    while (curr)
    {
        curr->target = target_the_a(curr, a);     // its landing spot in A
        cb = signed_rot(curr->pos, b->size);        // cost to bring curr to B-top
        ca = signed_rot(curr->target->pos, a->size);// cost to bring target to A-top
        if ((ca >= 0) == (cb >= 0) && abs_i(ca) > abs_i(cb))
            curr->cost = abs_i(ca);                 // same direction → overlap, take the larger
        else if ((ca >= 0) == (cb >= 0))
            curr->cost = abs_i(cb);                 // same direction → larger is ca? no, take cb
        else
            curr->cost = abs_i(ca) + abs_i(cb);     // opposite directions → no overlap, sum
        curr = curr->next;
    }
}
```

For each element in B, compute two signed rotation costs — `cb` (bring it to B's
top) and `ca` (bring its A-target to A's top). The clever part is the **combined
cost**:

- If `ca` and `cb` point the **same** direction (both up or both down), the
  rotations **overlap** — we can use `rr`/`rrr` for the shared part, so the cost is
  just the *larger* of the two magnitudes (`max(|ca|,|cb|)`).
- If they point **opposite** directions, no overlap — the cost is the **sum**
  `|ca| + |cb|`.

`(ca >= 0) == (cb >= 0)` is a compact "same sign?" test. The result is stored in
`curr->cost`.

> The two same-direction branches together implement `max(|ca|, |cb|)`: the first
> handles `|ca| > |cb|`, the second the remaining same-direction case.

**`find_cheapest` (lines 53–67)** — pick the minimum-cost B-node:

```c
t_node *find_cheapest(t_stack *b)
{
    t_node *best = b->head, *curr = b->head;
    while (curr)
    {
        if (curr->cost < best->cost) best = curr;
        curr = curr->next;
    }
    return (best);
}
```

Linear scan for the cheapest node to move next.

### `complex.c` — driving the moves

**`rotate_common` (lines 15–29)** — do the overlapping rotations with combined ops:

```c
while (*ca > 0 && *cb > 0) { rr(a, b);  (*ca)--; (*cb)--; }   // both up  → rr
while (*ca < 0 && *cb < 0) { rrr(a, b); (*ca)++; (*cb)++; }   // both down → rrr
```

While both costs share a direction, use the **combined** op (one op moves both
stacks). Decrements toward zero. After this, at most one of `ca`/`cb` is non-zero.

**`rotate_rest` (lines 31–53)** — finish whatever is left on each stack alone:

```c
while (ca > 0) { ra(a);  ca--; }
while (ca < 0) { rra(a); ca++; }
while (cb > 0) { rb(b);  cb--; }
while (cb < 0) { rrb(b); cb++; }
```

Single-stack rotations for the remaining offset on A and/or B.

**`move_cheapest` (lines 55–67)** — position and push the cheapest node:

```c
static void move_cheapest(t_stack *a, t_stack *b, t_node *bn)
{
    stack_o_meter(a); stack_o_meter(b);
    cb = signed_rot(bn->pos, b->size);           // recompute now (positions moved)
    ca = signed_rot(bn->target->pos, a->size);
    rotate_common(a, b, &ca, &cb);               // combined rotations
    rotate_rest(a, b, ca, cb);                   // leftover rotations
    pa(a, b);                                     // now the slot is aligned → push
}
```

**`final_rotate` (lines 69–78)** — rotate A's minimum to the top, cheaper direction:

```c
if (a->min->pos * 2 <= a->size)
    while (a->head != a->min) ra(a);
else
    while (a->head != a->min) rra(a);
```

**`complex_sorter` (lines 80–94)** — the whole thing:

```c
void complex_sorter(t_stack *a, t_stack *b)
{
    while (a->size > 3)          // 1. dump all but 3 into B (raw pushes)
        pb(a, b);
    three_sorter(a);             // 2. sort the base 3
    while (b->size > 0)          // 3. greedily return the cheapest each time
    {
        assign_cost(a, b);       //    price every B-node
        cheapest = find_cheapest(b);
        move_cheapest(a, b, cheapest);
    }
    final_rotate(a);             // 4. align the minimum to the top
}
```

The signature move: in phase 3 we **re-price all of B every iteration** and always
return the globally cheapest element, combining A/B rotations with `rr`/`rrr`
wherever their directions align. That combination is what gets the op count low
enough for the 500-element grader target.

> **Cost note:** `assign_cost` is O(n) and runs once per returned element, so phase
> 3 is ~O(n²) in the worst case. It's called "n log n-class" loosely because in
> practice the combined rotations keep the *operation count* near the n·log n
> benchmark, even if the *compute* is quadratic. Be precise about this distinction
> if pushed: **operation count** (what the subject grades) vs **runtime
> complexity** (CPU time). See `07`.

➡ Next: `06_main_and_dispatch.md`
