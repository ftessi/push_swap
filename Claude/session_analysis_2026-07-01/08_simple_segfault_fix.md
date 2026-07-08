# Simple sort — the segfault, and the fix — 2026-07-08

> You wired your teammate's `simple_sorter` into `sort_dispatch` and got a
> segmentation fault on `./push_swap 5 2 8 1`. I reproduced it, traced it with the
> debugger, and found **two** bugs (one crashes now, one would hang right after you
> fix the first). One small change fixes both. I verified the fix sorts correctly on
> 200 random 100-number inputs with 0 failures.

---

## 1. Reproducing it

```
./push_swap 5 2 8 1     ->  [exit 139]      (139 = 128 + 11 = SIGSEGV)
```
Debugger backtrace:
```
stop reason = EXC_BAD_ACCESS (code=1, address=0x8)
  frame #0: simple_sorter
  frame #1: sort_dispatch
  frame #2: main
```
`EXC_BAD_ACCESS address=0x8` = a **NULL-pointer dereference**. `0x8` is the byte
offset of the `pos` field inside `t_node` (after `int value` at 0 and `int index` at
4). So the crash is literally `NULL->pos` — i.e. `target_node` is **NULL** when the
code does `target_node->pos`.

---

## 2. Bug #1 — the crash: NULL target when B is empty

Phase 1 of `simple_sorter`:
```c
while (a->size > 3)
{
	stack_o_meter(a);
	stack_o_meter(b);
	target_node = target_the_b(a->head, b);   // <-- on the FIRST pass, b is EMPTY
	while (target_node->pos != 0)             // <-- CRASH: target_node is NULL
	{ ... }
	pb(a, b);
}
```

On the very first iteration **stack B is empty**. Follow the target lookup
(`algorithm_utils.c`):
```c
t_node	*target_the_b(t_node *node_a, t_stack_check *b)
{
	curr_b = b->head;              // b->head == NULL  (empty)
	target_node = NULL;
	while (curr_b) { ... }          // loop never runs
	if (target_node == NULL)
		return (b->max);           // returns b->max
	...
}
```
It returns `b->max`. But `b->max` is **NULL**: `init_stack` set it to NULL, and
`stack_o_meter` **returns early on an empty stack without setting `max`**:
```c
void	stack_o_meter(t_stack_check *stack)
{
	if (!stack || !stack->head)
		return ;                   // empty b -> max stays NULL
	stack->max = stack->head;
	...
}
```
So `target_node = NULL`, and `while (target_node->pos != 0)` dereferences NULL →
segfault. **That's your crash.**

---

## 3. Bug #2 — the one hiding behind it: a stale-`pos` infinite loop

Even if you guard the NULL, the rotation loops are still wrong:
```c
stack_o_meter(b);                     // sets every node's ->pos ONCE
target_node = target_the_b(a->head, b);
while (target_node->pos != 0)         // reads a CACHED pos
{
	if (target_node->median == true)
		rb(b);                        // rotates b, but does NOT update ->pos
	else
		rrb(b);
}
```
`->pos` is a field filled by `stack_o_meter`. The rotate ops (`rb`/`rrb`) move the
nodes but **don't touch `->pos`**. So `target_node->pos` keeps its old value forever —
if it wasn't already 0, the condition is *always* true → **infinite loop** (b spins
forever). The same mistake is in the push-back loop and in the final
`while (a->min->pos != 0)` loop.

You didn't see this yet only because Bug #1 crashes first.

---

## 4. The fix — compare live pointers, not stale positions

Replace every `while (X->pos != 0)` with a comparison against the stack's **live
head pointer**. The head pointer *does* change on every rotate, so the loop actually
makes progress and terminates:

| Old (broken) | New (fixed) |
|---|---|
| `while (target_node->pos != 0)`  (phase 1) | `while (b->head != target_node)` |
| `while (target_node->pos != 0)`  (phase 2) | `while (a->head != target_node)` |
| `while (a->min->pos != 0)`       (final)   | `while (a->head != a->min)` |

**Why this fixes Bug #1 too:** when B is empty, `target_the_b` returns `b->max` =
NULL, and `b->head` is *also* NULL. The condition `b->head != target_node` becomes
`NULL != NULL` → **false**, so the loop body never runs (no dereference), and you fall
straight through to `pb`. The first element just gets pushed — exactly what you want.
When B has one element and the lookup falls back to `b->max`, that equals `b->head`,
so again the loop skips and you `pb`. No special-casing needed.

**Why the direction stays correct:** `target_node->median` was set once by
`stack_o_meter` (top half → `true`). We *want* it fixed — we pick the shorter
direction once and rotate that way until the node reaches the top. Reading a constant
`median` each iteration is fine; it's only `pos` that must not be trusted as a live
counter.

### The corrected `simple.c`
Refactored into helpers so each function is < 25 lines and there are no comments
inside function bodies (both Norm rules). Behaviour is your teammate's algorithm,
just with the loop conditions fixed.

```c
#include "push_swap.h"

static void	push_all_to_b(t_stack_check *a, t_stack_check *b)
{
	t_node	*target_node;

	while (a->size > 3)
	{
		stack_o_meter(a);
		stack_o_meter(b);
		target_node = target_the_b(a->head, b);
		while (b->head != target_node)
		{
			if (target_node->median == true)
				rb(b);
			else
				rrb(b);
		}
		pb(a, b);
	}
}

static void	push_back_to_a(t_stack_check *a, t_stack_check *b)
{
	t_node	*target_node;

	while (b->size > 0)
	{
		stack_o_meter(a);
		stack_o_meter(b);
		target_node = target_the_a(b->head, a);
		while (a->head != target_node)
		{
			if (target_node->median == true)
				ra(a);
			else
				rra(a);
		}
		pa(a, b);
	}
}

void	simple_sorter(t_stack_check *a, t_stack_check *b)
{
	push_all_to_b(a, b);
	three_sorter(a);
	push_back_to_a(a, b);
	stack_o_meter(a);
	while (a->head != a->min)
	{
		if (a->min->median == true)
			ra(a);
		else
			rra(a);
	}
}
```

### Line-by-line of the parts that changed
- `push_all_to_b`: while A has more than 3 nodes — refresh metrics on both stacks;
  ask `target_the_b` where on B the current top of A belongs; rotate B (the cheap
  direction picked by `median`) **until that target node is the head** (`b->head !=
  target_node`); then `pb`. On the first pass B is empty so the inner loop is skipped
  and the element is just pushed — no crash.
- `push_back_to_a`: while B still has nodes — find in A the smallest value larger than
  B's top (`target_the_a`, which returns `a->min` when B's top is the biggest), rotate
  A until that target is on top, `pa`. Same pointer-comparison pattern.
- final loop in `simple_sorter`: A is sorted but maybe rotated; bring the minimum to
  the top with `while (a->head != a->min)`.

---

## 5. Verification

I built with this fix and simulated the emitted ops back onto the input:

```
[5 2 8 1]                 ->  OK,  6 ops
[6 5 4 3 2 1]             ->  OK, 12 ops
[3 1 2 5 4]               ->  OK, 10 ops
random 100-num x200       ->  0 failures, max 1623 ops
n = 3 / 5 / 100 / 500     ->  all OK  (500 -> ~30k ops)
```
All correct, no crash, no hang. The op counts (100 → ~1.6k, 500 → ~30k) are what you
expect from an **insertion-style O(n²) sort** — perfectly fine for the *simple*
strategy. That quadratic growth is exactly why you also need the **complex** sort for
big inputs → see `09_complex_turk_algorithm.md`.

---

## 6. What to do
1. Apply the corrected `simple.c` above. `make re`, retest `./push_swap 5 2 8 1`.
2. Run `norminette simple.c` — the refactor keeps functions short and comment-free.
3. Strip the debug `printf`s from `main.c` / `sort_dispatch` (and `#include <stdio.h>`)
   before committing — stdout must carry only operations. Use `write` to fd 2 for
   temporary debugging.
4. Commit: `"fix NULL/stale-pos loops in simple_sorter"`.

> Note for later: `stack_o_meter` leaving `min`/`max` = NULL on an empty stack is the
> root enabler of Bug #1. The pointer-comparison fix sidesteps it, so you don't have
> to change `stack_o_meter` — but don't call `->max`/`->min` on a possibly-empty stack
> elsewhere without a guard.
