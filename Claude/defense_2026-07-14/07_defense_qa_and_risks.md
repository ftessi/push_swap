# 07 — Defense Q&A, Complexity, and Known Risks

Read this last, and read it twice before the defense. Part 1 is the honest list of
things that could cost points — fix or be ready to explain each. Part 2 is a
question bank with answers. Part 3 is the complexity story.

---

## Part 1 — Known risks (in priority order)

### 🔴 R1. `printf` / `<stdio.h>` are used — forbidden functions

- **Where:** `main.c` (lines 2–3 include `<stdio.h>`, `print_benchmark_summary`
  calls `printf`), and `simple.c` line 14 includes `<stdio.h>` (unused).
- **Why it matters:** the mandatory push_swap allowed functions are `write`,
  `read`, `malloc`, `free`, `exit`, plus your own `ft_printf`/libft. `printf` is
  **not** allowed. A binary that links `printf` fails the automated check.
- **Fix options:** (a) remove `print_benchmark_summary` and the `--bench` feature
  for submission; or (b) rewrite it with `write` + a small integer-to-string
  helper, printing to **stderr (fd 2)** not stdout; and remove the unused
  `<stdio.h>` from `simple.c`.
- **If asked:** "The bench summary is a dev tool; for the graded build we strip
  `printf` and print stats via `write` to stderr so the checker pipe stays clean."

### 🔴 R2. The strategy flags don't drive selection

- **Where:** `sort_dispatch` (`main.c`) routes by `a->disorder` and never reads
  `a->strategy`.
- **Effect:** `--simple`/`--medium`/`--complex`/`--adaptive` are parsed but change
  nothing. `--simple` on random data still runs complex.
- **Fix (~5 lines):** branch on strategy first, disorder as the ADAPTIVE fallback:
  ```c
  if (a->size <= 3) { small-case }
  else if (a->strategy == SIMPLE)  simple_sorter(a, b);
  else if (a->strategy == MEDIUM)  medium_sorter(a, b);
  else if (a->strategy == COMPLEX) complex_sorter(a, b);
  else /* ADAPTIVE */              dispatch-by-disorder;
  ```
- **If asked:** answer honestly (see doc `06`). Don't claim the flags work if they
  don't — evaluators test exactly this.

### 🟠 R3. Comments inside function bodies — Norm violation

- **Where:** almost every `.c` file (e.g. `ops_swap.c`, `stack_utils.c`, the
  Turkish comment in `main.c` line 53).
- **Why:** norminette forbids comments inside a function body; they must be between
  functions or at file top. Also several are end-of-line comments.
- **Fix:** run `norminette Code/` and strip/relocate every in-function comment
  before the graded defense. (Keep this documentation folder as your reference
  instead.)

### 🟠 R4. Memory leak of stack B on error-exit paths

- **Where:** `free_and_exit` (`parser.c`) frees only the stack it's given (A). B is
  allocated in `main` before `build_stack`, so on an invalid input B is never
  freed.
- **Effect:** `valgrind ./push_swap 1 2 x` reports B as still reachable. On valid
  runs there is no leak (main frees both).
- **Fix:** either allocate B *after* successful parsing, or pass both stacks to the
  error path, or `free` B in `main` isn't reachable because we `exit`. Simplest:
  build A fully first, allocate B only right before sorting.

### 🟡 R5. `ft_atoi_ps` doesn't advance past a leading `'+'`

- **Where:** `parser.c` lines 60–67: only `'-'` triggers `i++`.
- **Effect:** for `"+5"`, the loop's first iteration processes `'+'`
  (`'+' - '0'` = -5 added to res). In practice `error_syntax` runs first and
  *allows* `+`, so `"+5"` reaches atoi and would miscompute. **Test `./push_swap +5`
  before the defense** — if it misbehaves, add `i++` for the `'+'` branch too.
- **Note:** worth a 2-minute check; may already be masked depending on inputs.

### 🟡 R6. `--bench` output goes to stdout

Covered in R1 — if benchmarking is kept, move it to stderr so
`./push_swap --bench $ARG | ./checker $ARG` doesn't feed stats to the checker.

### 🟢 R7. Non-official flags

The `--simple/--medium/--complex/--adaptive/--bench` flags are **not** in the
official push_swap subject (which has no flags). They're a team extension. Harmless
for the mandatory grade as long as a plain `./push_swap <numbers>` works and the
forbidden-function issue (R1) is resolved. Just don't present them as required.

---

## Part 2 — Question bank

**Q: Why a doubly-linked list instead of an array?**
A: The moves act on both ends — `ra`/`pb` at the top, `rra`/`rrb` at the bottom.
`prev` pointers make detaching the tail O(1). An array would need shifting or
index bookkeeping for rotations; the list keeps every op O(1) with clean pointer
surgery.

**Q: What is "disorder"?**
A: The fraction of value-pairs that are inverted (i above j but larger). 0 = sorted,
~0.5 = random, ~1 = reversed. Computed once, O(n²), in `compute_disorder`. We use
it to choose a strategy.

**Q: How do you sort 3 elements optimally?**
A: `three_sorter`: send the max to the bottom (`ra` if on top, `rra` if in the
middle), then `sa` if the top two are still out of order. ≤2 ops for any of the 6
permutations. (Walk the table in doc `04`.)

**Q: How does the complex/Turk sort keep the op count low?**
A: It prices every element in B by the rotations needed to align it *and* its
target in A, using signed costs so it knows the direction. Same-direction A/B
rotations overlap into one `rr`/`rrr` op. It always moves the globally cheapest
element next. That op-combining is the whole trick.

**Q: What keeps stacks A and B ordered during the sort?**
A: Invariants: **A ascending, B descending.** `target_the_a`/`target_the_b` find
the correct insertion slot to preserve those, so pushing back is cheap.

**Q: What happens on invalid input?**
A: `error_syntax` (non-numeric / lone sign / empty), `ft_atoi_ps` (out of int
range), and `error_duplicate` each route to `free_and_exit`, which frees, writes
`"Error\n"` to stderr, and `exit(1)`.

**Q: What does `./push_swap` with no args do? With one number? Already sorted?**
A: No args → exit 0, nothing printed. One number / already sorted → `is_sorted` is
true (or we never enter dispatch) → zero ops, exit 0.

**Q: Why compute disorder before sorting?**
A: Operations mutate the stack; the metric must describe the *original* input, and
the strategy choice must be made on it.

**Q: Where do the operation counts come from?**
A: Each op wrapper (`sa`, `pb`, …) increments a counter on the stack it touched;
`--bench` sums them. `ss`/`rr`/`rrr` count as one op each.

**Q: What's your worst case and roughly how many ops for 100 / 500 elements?**
A: (Fill in with your measured numbers — run
`./push_swap $(python3 -c "import random;print(' '.join(map(str,random.sample(range(-1000,1000),100))))") | wc -l`
a few times.) Targets are ≈700 for 100 and ≈5500 for 500 to score full marks;
know your actual averages.

---

## Part 3 — Complexity (be precise: op-count vs runtime)

Two different "costs" — the subject grades **operation count**, not CPU time.

| Piece | Runtime (CPU) | Notes |
|-------|---------------|-------|
| Parsing (`build_stack` + `error_duplicate`) | O(n²) | duplicate check scans the list per insert |
| `compute_disorder` | O(n²) | all pairs |
| `stack_o_meter` | O(n) | one pass; called many times |
| `simple_sorter` | ~O(n²) ops & runtime | per-element target + rotations |
| `medium_sorter` | ~O(n√n) ops | chunking cuts return cost; ranking is O(n²) runtime |
| `complex_sorter` | op-count near n·log n; **runtime O(n²)** | re-prices all of B each of n returns |

**Key defense sentence:** "Our *operation counts* land in the expected classes
(that's what's graded), but some *runtime* is quadratic — e.g. duplicate detection
and re-costing B each iteration. For the input sizes push_swap targets (≤500),
quadratic runtime is instant; the operation count is what we optimized."

---

## Part 4 — 10-minute pre-defense checklist

- [ ] `make re` compiles clean with `-Wall -Wextra -Werror`.
- [ ] `norminette Code/` — strip in-function comments (R3).
- [ ] Decide on `--bench`: remove it or move it to `write`+stderr (R1/R6); drop
      unused `<stdio.h>` in `simple.c`.
- [ ] `./push_swap 3 2 1 | ./checker 3 2 1` → **OK**; big random too.
- [ ] `./push_swap 2 1 3 4 3` / `1 2 x` / `+5` / `9999999999` → **Error**.
- [ ] `./push_swap` and `./push_swap 42` → no output, exit 0.
- [ ] `valgrind` clean on a valid run; know the B-leak on error paths (R4).
- [ ] Each teammate can explain `three_sorter` and the complex cost model unaided.
- [ ] Be ready to say plainly: the selector flags don't currently change the
      algorithm (R2).

---

### Bottom line for the defense

The **core project is solid**: correct doubly-linked-list model, clean O(1) ops,
three real strategies, disorder-driven selection, all subject error cases handled.
The gaps are (1) the forbidden `printf` in the optional bench path, (2) the selector
flags not wired into dispatch, and (3) norminette comments. None affect a plain
`./push_swap <numbers> | ./checker <numbers>` run — fix R1/R3 before submission and
be transparent about R2, and you defend from a position of strength.
