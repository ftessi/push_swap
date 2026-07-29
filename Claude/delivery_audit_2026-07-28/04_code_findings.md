# Code Findings

Findings from a full read of all 18 source files plus targeted testing. Nothing
here is a sorting bug — the checker never returned `KO`.

---

## 1. `free_and_exit` does not free the `t_stack` struct

**File:** `parser.c:103-121`

```c
	if (stack)
		free_stack(stack);      /* frees the nodes, not the struct */
	write(2, "Error\n", 6);
	exit(1);
```

`free_stack` walks the node list and frees each `t_node`, then nulls `head`/`tail`.
The `malloc(sizeof(t_stack))` from `init_stack` is never released.

**Impact:** valgrind "still reachable", ~120 bytes, on every error path. Process
exit reclaims it. Low severity, but `main.c`'s `clean_exit` already does it right,
so this is pure inconsistency. Fix in `03_blockers.md` §5.

**Also note:** `free_stack` early-returns when `stack->head == NULL`
(`stack_utils.c:105`), so `size` is not reset in that case. Nothing currently
depends on it, but it is a small landmine if the function is ever reused.

## 2. `--bench` reports the strategy that *would* run, not the one that ran

**Files:** `main.c:39-63`, `benchmark.c:78-97`

`sort_dispatch` short-circuits: size 2 → `sa`, size 3 → `three_sorter`, and
`main` skips sorting entirely when `is_sorted(a)`. But `print_strategy_fd` derives
its label purely from `a->strategy` and `a->disorder`.

Observed:
```
$ ./push_swap --bench 5 3 1
sa / ra on stdout
STRATEGY | Adaptive (Complex) / O(n log n)     ← complex_sorter never ran
```

Same for an already-sorted input: it prints a strategy while zero algorithm
executed. Cosmetic, but an evaluator poking at `--bench` will spot it in ten
seconds. If you want it exact, record the branch actually taken in `sort_dispatch`
(a `t_strategy` field set at dispatch time, or a small `resolved_strategy` member)
and have the benchmark read that.

## 3. Adaptive low-disorder regime blows past 12000 ops at n = 500 ⚠️

**Files:** `main.c:55-60`, `algo_simple.c`

The subject *mandates* `disorder < 0.2 → O(n²)`, and `main.c` obeys it. The problem
is that `simple_sorter` is O(n²) **and does not exploit near-sortedness cheaply**:
it pushes everything to B one at a time, rotating B to the insertion point each
time. Cost tracks disorder roughly linearly and gets expensive well before 0.2:

| disorder | ops at n=500 |
|----------|--------------|
| 0.05 | 7 102 |
| 0.08 | 10 170 |
| 0.10 | **12 240** |
| 0.16 | **17 298** |
| 0.19 | **19 138** |

So the band `0.09 ≲ d < 0.20` at n = 500 exceeds the subject's own 12000-op *pass*
threshold — using the subject's own mandated strategy.

**Is this a violation?** Strictly, no. VI.6 says *"for 500 **random** numbers"*, and
random input has disorder ≈ 0.5, which routes to the complex sorter (5115 avg). You
are compliant. But it is the sharpest question an evaluator can ask.

> **⚠️ Correction (2026-07-29).** This section originally suggested an LIS-based
> rewrite ("extract only the out-of-place elements") would fix this. It does not —
> see `06_changes_applied.md` §3 for the cost analysis. Extraction gets cheap, but
> stack A stays large, so re-inserting the `n − k` extracted elements costs O(k) each
> and lands in the same 10⁴ range. Getting under 12000 requires cost-based selection,
> which *is* the complex sorter — porting it here would collapse two of the four
> required strategies into one. **The code was deliberately left unchanged.**

**How to handle it:** defend it. "The subject requires O(n²) below 0.2. O(n²) is an
*upper bound* and our simple sorter meets it. The benchmark in VI.6 is specified on
random input, where disorder ≈ 0.5 routes to the O(n log n) strategy and we average
~5090 operations. Any O(n²)-shaped algorithm costs ~10⁴ operations at n=500 — that is
what the complexity class costs, not a defect in our implementation."

This is now stated in the README under "Known trade-off, stated honestly". Volunteer
it rather than waiting to be asked.

## 4. Medium's chunk size is a constant, not `√n`

**File:** `algo_medium.c:129-141`

```c
	if (a->size <= 100)
		chunk_size = 12;
	else
		chunk_size = 30;
```

The O(n√n) claim rests on there being ~`√n` chunks of ~`√n` elements. That holds at
n=100 (12 ≈ √100) and is close at n=500 (30 vs √500 ≈ 22). For a *fixed* constant `c`
the true class is O(n²/c), which is O(n²) asymptotically.

Measured performance is excellent (574 @ 100, 5056 @ 500), so this is a **defense
risk, not a performance problem**. Two options:

- Compute it: `chunk_size = isqrt(a->size) + k` — makes the complexity claim literal
  and costs you one small integer-sqrt helper.
- Or keep the table and be ready to say: *"we tuned constants for the sizes the
  subject benchmarks; the asymptotic argument uses chunk ≈ √n, and our constants
  approximate it at n=100 and n=500."*

The first is safer. If you keep the constants, **make sure the README's complexity
argument does not claim something the code does not do.**

`send_to_b` itself is sound — I verified it cannot loop forever: `idx` counts pushes,
and every index below the minimum remaining index has already been pushed, so
`min_index ≤ idx ≤ idx + chunk_size` always holds and progress is guaranteed.

## 5. Complex sorter is O(n³) in CPU time

**File:** `algo_complex.c:117-122` + `algo_complex_utils.c:54-76`

`assign_cost` is called once per element removed from B, and it scans all of B while
calling `target_the_a` (a full scan of A) for each node ⇒ O(n²) per iteration,
O(n³) overall. Measured: n=1000 → 0.06 s, n=2000 → 2.96 s, n=5000 → > 120 s.

**This is not a subject violation.** VI.3.1 is explicit: *"When you state a complexity
class, it must reflect the cost measured in **number of push_swap operations
generated**, not the theoretical complexity of a classical array-based algorithm."*
Our *operation* count at n=500 is ~5100 ≈ n·log₂n. The claim is correct as stated.

Just don't volunteer a 5000-element demo.

## 6. Magic numbers instead of the enum

**Files:** `main.c:47-52`, `benchmark.c:81-86`

```c
	if (a->strategy == 1)          /* SIMPLE */
	else if (a->strategy == 2)     /* MEDIUM */
	else if (a->strategy == 3)     /* COMPLEX */
```

`t_strategy { ADAPTIVE, SIMPLE, MEDIUM, COMPLEX }` exists in `push_swap.h:32-38` and
is used correctly in `flags.c`. Using literals here works only because the enum
happens to be ordered that way — reorder the enum and both files silently break.
Swap in the names. Zero risk, and an evaluator asking "what is `strategy == 2`?" is
a question you shouldn't have to field.

## 7. Duplicated include and comment block

**File:** `algo_simple.c` — `#include "push_swap.h"` at lines 13 **and** 39; the same
"ALGORITHM SUMMARY" comment at lines 15-37 **and** 41-62. Delete 39-62.

## 8. Multi-byte characters in `write()` literals

**File:** `benchmark.c:84,93` — `"Medium / O(n√n)\n"` with a hardcoded length of 18.
`√` is 3 bytes in UTF-8, so 18 is correct (I counted each of them; all the hardcoded
lengths in `benchmark.c` check out). Norminette accepts it and the output renders
fine.

Flagging it only so you know *why* those numbers look off by two — if anyone edits
those strings, the byte counts must be recomputed by hand. A `ft_putstr_fd` helper
would remove the whole class of error.

## 9. Multiple quoted strings are rejected

**File:** `build_stack.c:114-138`

`nums == 1` → treat the single argument as a space-separated list and split it.
`nums > 1` → treat each argument as one number.

So `./push_swap "4 67 3" "87 23"` → `Error`. The subject does not require handling
mixed forms, and `./push_swap "" 1` → `Error` matches the PDF example exactly, so
this is **compliant**. Some peer test suites do try the mixed form, though. If you
want to be safe, split *every* argument on spaces regardless of count — it collapses
both branches into one loop and is strictly more permissive.

Related: the split uses `' '` only, so tab-separated input inside one string fails.
Not required.

## 10. Dead file in the tree

`Code/tests/bench_main.c` is 114 lines of entirely commented-out code, tracked in
git, and not in `SRC`. Delete it. If you want to keep a manual test harness, keep it
in a branch or outside the graded tree.

---

## Things I checked that are genuinely correct

Worth knowing so you can answer confidently:

- **`ft_atoi_ps` overflow guard** (`parser.c:93-99`) — the check runs *inside* the
  digit loop, so `res` can never exceed ~2.1×10¹⁰ before bailing. No `long` overflow
  even on a 40-digit input. Verified with `1 99999999999`.
- **Operation semantics** — `sa/sb/ss` no-op below 2 elements, `pa/pb` no-op on empty
  source, `ra/rra` correctly maintain `head`, `tail`, `prev`, `next` in the
  doubly-linked list. Exhaustive testing would have caught any pointer error.
- **Disorder is measured before any move** — `main.c:99`, after `build_stack`, before
  `sort_dispatch`. Exactly what VI.3.2 demands.
- **B is allocated only after A validates** — `main.c:87-98`, so an input error can't
  leak B.
- **No global variables** — all mutable state is inside `t_stack`, passed by pointer.
- **Op counters** — `pb` increments `b->op_pb`, `pa` increments `a->op_pa`, and the
  dual ops (`ss`, `rr`, `rrr`) are counted once on `a`. `print_benchmark_summary`
  sums exactly the right eleven fields. No double counting.
