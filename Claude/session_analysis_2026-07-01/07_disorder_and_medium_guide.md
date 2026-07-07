# Disorder metric + road to the medium sort — 2026-07-07

> You wired `compute_disorder` into `main` and saw garbage in one of the prints. This
> guide: (1) what's actually wrong (it's **not** what you think), (2) the one-line fix,
> (3) how to wire disorder in cleanly, (4) what disorder is *for*, (5) the step ladder
> from here into the **medium O(n√n)** sort while your teammate builds the simple one.
> I built with `make re` and ran `./push_swap 6 5 4` and others — results below.

---

## 1. Is the disorder function correct? — YES, the logic is right

I ran it on many inputs. The **algorithm is correct**: it counts *inversions* (pairs
where an earlier value is bigger than a later one) divided by *total pairs*. That's a
proper normalized 0..1 disorder metric (Kendall-tau distance). Verified:

| Input | Expected | What the FIRST print showed |
|-------|----------|------------------------------|
| `6 5 4` (fully reversed) | 1.0 | `1.000000` ✓ |
| `4 5 6`, `1 2 3` (sorted) | 0.0 | `0.000000` ✓ |
| `3 1 2` | 0.667 | `0.666667` ✓ |
| `5 2 8 1 9` | 0.4 | `0.400000` ✓ |
| `9 1 8 2 7 3` | 0.6 | `0.600000` ✓ |

So the math is fine. **Do not rewrite the algorithm.**

---

## 2. The real bug — and it is NOT uninitialized stack values

You said: *"there's garbage values inside the stack because I haven't initialized those
values."* That diagnosis is **wrong**, and it matters — chasing it would send you
editing `node_creator`/`init_stack` for no reason. Here's the proof: the **first**
print (`Disorder context`) was **correct every time**. If the stack node values were
garbage, the first print would be garbage too. The values come straight from your
parser — they're fine.

The garbage is one thing only: **the local variable `mistakes` in `compute_disorder`
is left uninitialized on the normal path.** Look:

```c
if (!stack || stack->size < 2)
	mistakes = 0.0;      // <-- mistakes is ONLY set here
total_pairs = 0.0;
...
	mistakes += 1.0;     // <-- for size >= 2, you += onto a garbage starting value
```

When `size >= 2` (every real input), `mistakes` starts as whatever garbage is on the
stack frame. Why did your **first** call look right but the **second** print explode
to `2.6e+69`? Because between the two calls you ran `printf`, which overwrote that
stack-frame slot with its own leftover bytes — so the second `compute_disorder` began
from that garbage. Same bug, two different garbage values.

This is undefined behavior. It slips past `-Wall -Wextra` because the relevant warning
(`-Wconditional-uninitialized`) isn't in `-Wall` — I confirmed it triggers under that
flag back in file `04`.

### The fix (one line, plus a tidy)
Initialize `mistakes` unconditionally, like `total_pairs`. Then a clean early return
handles the `size < 2` case (and prevents a `0 / 0` division):

```c
double	compute_disorder(t_stack_check *stack)
{
	t_node	*i;
	t_node	*j;
	double	mistakes;
	double	total_pairs;

	mistakes = 0.0;                       /* <-- the fix */
	total_pairs = 0.0;
	if (!stack || stack->size < 2)
		return (0.0);                     /* nothing to compare, no div-by-zero */
	i = stack->head;
	while (i)
	{
		j = i->next;
		while (j)
		{
			total_pairs += 1.0;
			if (i->value > j->value)
				mistakes += 1.0;
			j = j->next;
		}
		i = i->next;
	}
	return (mistakes / total_pairs);
}
```

I tested this exact version: `6 5 4 → 1.0`, sorted `→ 0.0`, `3 1 2 → 0.667`,
`5 2 8 1 9 → 0.4`, `9 1 8 2 7 3 → 0.6`, single `42 → 0.0` — and now **both** prints
match. (I reverted the file so you apply it yourself.)

Note: this is still **O(n²)** (double loop). Totally fine for the disorder metric —
don't optimize it. Just be ready to say "it's O(n²) but only runs once, on ≤ a few
hundred numbers" at defense.

---

## 3. Wire it into `main` cleanly

Your current `main` has three things to fix beyond the bug above:

1. **You call `compute_disorder(a)` twice.** Call it **once**, store in `a->disorder`.
2. **Remove the debug `printf` and `#include <stdio.h>`** before this is "real."
   `push_swap` must print **only operations** to stdout — a stray `printf` there fails
   evaluation. Keep the prints only while eyeballing, then strip them. (To debug
   without polluting stdout, print to stderr: `fprintf(stderr, ...)` or a `write` to
   fd 2 — stderr is ignored by the graders' op-counter.)
3. **Norm:** the comments inside `sort_dispatch` (in-function comments are forbidden),
   the `if (argc < 2)` / `return (0)` indentation, and trailing whitespace.

Target shape (debug removed):

```c
int	main(int argc, char **argv)
{
	t_stack_check	*a;

	if (argc < 2)
		return (0);
	a = init_stack();
	if (!a)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	build_stack(a, argc, argv);
	a->disorder = compute_disorder(a);
	if (!is_sorted(a))
		sort_dispatch(a);
	free_stack(a);
	free(a);
	return (0);
}
```

You're computing disorder in the **right place**: once, right after the stack is
built, *before* any moves. Keep it there.

### Verify checklist (disorder)
- `./push_swap 1 2 3`, `4 5 6` → 0.0
- `./push_swap 6 5 4`, `3 2 1` → 1.0
- `./push_swap 3 1 2` → ~0.667; `5 2 8 1 9` → 0.4
- `./push_swap 42` and no-arg → 0.0, and **no leak** (`valgrind`).
- With debug prints kept temporarily: both prints identical.

---

## 4. What is disorder actually FOR? (so this isn't busywork)

Right now `a->disorder` is computed and stored but **drives nothing** — dispatch still
switches on `size`. That's fine for today. Its real job comes at the **adaptive
strategy** (`11_strategy_adaptive.md`): the program will read `a->disorder` and pick
which sorter to run. Rough intent:

- **Low disorder** (nearly sorted) → a cheap **simple/insertion** pass wins; a fancy
  sort would waste ops.
- **High disorder + large n** → **complex (radix / O(n log n))**.
- **Middle ground** → **medium (O(n√n))**.

So the metric is the *input to a decision* you'll make later. Getting it correct and
stored now is exactly the right groundwork. Don't hook it into dispatch yet — first you
need the strategies to choose between.

---

## 5. From here → the medium O(n√n) sort

Your teammate owns **simple** (`08`). You take **medium** (`09`, O(n√n) chunk sort).
Read `09_strategy_medium.md` for the theory; this is the concrete order of operations
so you don't get stuck.

### 5.0 Two things `main`/dispatch need first
- **Stack B.** Simple and medium both use `pb`, so they need a second stack. Today
  `main` only builds `a` and `sort_dispatch(a)` takes one arg. Grow it to build `b`
  too and pass both: `sort_dispatch(a, b)`. Free both at the end.
- **A dispatcher that scales.** Plan the shape now:
  ```
  size == 2            -> sa
  size == 3            -> three_sorter
  size <= 5 (small)    -> teammate's simple_sorter(a, b)
  else                 -> medium_sorter(a, b)   (your job)
  ```
  (Later the `else` becomes the adaptive choice using `a->disorder`.)

### 5.1 Prerequisite: ranks (indices), not raw values
Chunk sort works on **ranks** `0 .. n-1`, not the user's numbers — that's what makes
"which chunk is this in?" trivial and immune to huge/negative values. Your node has an
`index` field but **nothing fills it yet** (`stack_o_meter` sets `pos/min/max/median`,
not `index`).

Write a **rank-assignment** step. Two common ways:
- **Count-smaller (O(n²), simplest):** for each node, its rank = how many values in the
  stack are smaller than it. Fine for n ≤ 500.
- **Sort-a-copy (O(n log n)):** copy values to an array, sort it, then each node's rank
  = its position in the sorted array. Do this if you want the cheaper option.

Verify: after ranking, the ranks are a permutation of `0..n-1` with no gaps/dupes.

### 5.2 Reusable helper: "bring rank R to the top the cheap way"
You'll use this constantly. Given a target node, compare its `pos` to `size / 2`:
- top half → rotate up (`ra`/`rb`) that many times;
- bottom half → reverse-rotate (`rra`/`rrb`) `size - pos` times.
Choosing the shorter direction is what keeps the op count down — skipping it roughly
doubles your ops.

### 5.3 Phase 1 — push A into B in chunks
- Chunk size `cs ≈ √n` (start bigger for correctness, tune later). Chunk 0 = ranks
  `[0, cs)`, chunk 1 = `[cs, 2cs)`, …
- Rotate through `a`. For the element on top:
  - if its rank is in the **current chunk** → `pb` (optional refinement: if it's in the
    lower part of the chunk, `rb` after to keep `b` roughly ordered);
  - else → `ra` to skip it.
- Advance chunk by chunk until `a` is empty.

### 5.4 Phase 2 — push B back to A, largest first
- `b` holds everything; because you parked by chunk, the **max** is always near the
  top. Repeatedly: find current max in `b`, bring it to the top the cheap way, `pa`.
- Pushing the largest each time leaves `a` sorted ascending (smallest on top).

### 5.5 Test then tune
1. Correctness first with a generous `cs` (e.g. `cs = n/5`): `./push_swap` a shuffled
   0..99 and check the output sorts it (write/borrow a checker — file `15`/`16`).
2. Then push `cs` toward `√n` and **count ops**: 100 numbers should land well under
   the "good" threshold; doubling n should grow ops ~2.8×, not 4× (proves you're
   sub-quadratic, not secretly O(n²)).

### 5.6 Norm / file layout
Max **5 functions per file**. Medium won't fit in one file — split e.g.
`medium.c` (the two phases + entry `medium_sorter`) and `medium_utils.c`
(rank assignment, "bring rank to top", find-max-in-b). Add both to the Makefile `SRC`.
No comments inside function bodies; tabs; ≤25 lines/function.

---

## 6. Immediate next actions (in order)
1. Apply the one-line `mistakes = 0.0` fix in `disorder.c` (+ early return). Rebuild,
   run the §3 checklist. Commit: `"fix uninitialized mistakes in compute_disorder"`.
2. Clean `main`: call `compute_disorder` once, store in `a->disorder`, remove
   `printf`/`<stdio.h>`, fix the Norm issues. Commit.
3. `norminette *.c *.h` clean. Commit.
4. Add stack **B** to `main` + widen `sort_dispatch(a, b)` with the ladder in §5.0.
5. Rank assignment (§5.1) + "bring rank to top" helper (§5.2) — these are shared with
   the simple sort, so coordinate with your teammate to write them **once**.
6. Build medium phases (§5.3–5.4), test, tune (§5.5).

➡ After medium: `10_strategy_complex.md` (radix / O(n log n)), then
`11_strategy_adaptive.md` where `a->disorder` finally picks the strategy.
