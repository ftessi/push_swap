# Changes Applied (2026-07-29)

Follow-up to the audit in this folder. Every blocker from `03_blockers.md` and every
finding from `04_code_findings.md` was either fixed or explicitly decided against —
with the reasoning here, because you have to defend these choices, not just ship them.

**Net effect:** 175 insertions, 318 deletions across 12 files, 3 new files, 2 deleted.
Fewer lines than before, and every verification that passed on 2026-07-28 still passes.

---

## Re-verification after the changes

| Check | Before | After |
|-------|--------|-------|
| `cc -Wall -Wextra -Werror` | clean | clean |
| Relink | none | none |
| Norminette | 18/18 OK | **20/20 OK** (2 new files) |
| Exhaustive n=1..7 × 5 modes | 0 failures | **0 failures** |
| Random stress (200 runs, negatives) | 0 failures | **0 failures** |
| Leaks (all error paths) | 0 | 0 |
| 100 random, default | 577 | **574** |
| 500 random, default | 5115 | **5090** |

Both benchmark numbers stay in the *excellent* band. Nothing regressed.

---

## Blockers

### ✅ 1. README.md — written

Now at `Code/README.md`, which is the root of the school repository once you upload
the contents of `Code/`. It covers every item Chapter VII demands:

- the italicised first line naming both logins
- Description, Instructions, Resources
- a detailed explanation **and justification** of all four algorithms
- the adaptive thresholds, the technique chosen per regime, and an upper-bound
  complexity argument for **time and space** in the push_swap operation model
  (Chapter VI.3.3.4 asks for this specifically and it is easy to miss)
- both learners' contributions
- a specific, honest account of how AI was used

**One thing you must check yourself:** the Contributions table. Git shows both of you
on essentially every file, so I wrote it as shared ownership with a lead/reviewer split
inferred from commit counts. If that does not match what actually happened, fix it —
this is the one section I cannot verify and the one an evaluator may ask you about
directly.

The README also documents the multiple-quoted-string input form and the `--bench`
output format, both of which changed today.

### ➖ 2. Repository layout — skipped, by your instruction

Left `Code/` alone. Since you upload the *contents* of `Code/` to the school
repository, `README.md` was placed inside `Code/` so it lands at that repo's root
where Chapter VII requires it.

Two things to remember when you push to school:
- **Do not upload `checker_Mac`.** It is the school-provided binary, not your work.
  It is still in `Code/` because you need it locally for testing.
- `Code/.DS_Store` and `Code/tests/bench_main.c` were deleted (see finding §10).

### ✅ 3. `--bench` disorder is now a percentage

`benchmark.c` → `ft_putpercent_fd`. Output changed from `0.13` to `13.00%`.

**Why it mattered beyond the letter of the spec:** the old code printed the raw ratio
with two decimals, so anything under 1% disorder collapsed to `0.00` and looked broken
during a demo. The new helper scales to hundredths of a percent *before* rounding, so
a disorder of 0.0182 now prints `1.82%` instead of `0.02`.

### ✅ 4. `CC = gcc` → `CC = cc`

Chapter II says `cc`. Same binary on macOS, one word, free point.

### ✅ 5. `free_and_exit` frees the stack structure

`parser.c` — was freeing every node but leaking the `t_stack` itself on every error
path. Valgrind would report it as *still reachable* rather than *definitely lost*, so
it was unlikely to fail you, but `main.c`'s `clean_exit` already did it correctly and
the inconsistency was the only reason it survived. Safe because `free_and_exit` always
`exit()`s, so no caller can hold a dangling pointer.

### ✅ 6. Duplicated block in `algo_simple.c`

Removed the second `#include "push_swap.h"` and the repeated 20-line comment. Replaced
with a complexity note that says what the file actually costs.

---

## Code findings

### ✅ §1 Stack struct leak — fixed
See blocker 5 above.

### ✅ §2 `--bench` reported a strategy that never ran — fixed

This one turned into the most useful change of the day, because fixing it properly also
killed the duplicated-threshold trap flagged in `05_defense_prep.md` §4.

**Before:** the thresholds `< 0.20` / `< 0.50` were written out twice — once in
`main.c` to dispatch, once in `benchmark.c` to print. Change one and the benchmark
would confidently report a strategy the program did not run. Separately, a 3-element
stack or an already-sorted stack still printed a strategy name even though no algorithm
executed.

**After:** one public function, `resolve_strategy(a)` in `main.c`, is the single source
of truth. `sort_dispatch` calls it to decide what to run and records the result in the
new `a->resolved` field; `benchmark.c` calls the *same* function to print `SELECTED`
and reads `a->resolved` for `EXECUTED`. They cannot disagree.

The enum gained `TINY` (the hardcoded 2-3 element micro-sort) and `NONE` (nothing ran,
input already sorted). Neither is user-selectable — they only ever appear in `resolved`.

```
         | SELECTED : Complex / O(n log n)
         | EXECUTED : Direct micro-sort / O(1)
```

That is the honest answer for `--complex --bench 3 2 1`, and it still satisfies the
subject's requirement that the selected strategy and its class be available in `--bench`.

### ⛔ §3 Low-disorder O(n²) blowup at n=500 — investigated, deliberately not changed

**This is a correction to my own earlier note.** `04_code_findings.md` §3 suggested an
LIS-based rewrite ("extract only the out-of-place elements") would fix this. I worked
through the cost model before touching the code, and **it does not.**

The reason: extracting only the non-LIS elements makes the *extraction* phase cheap
(n operations instead of ~n²/8), but it leaves stack `a` large — it keeps the whole
increasing subsequence. Every one of the `n − k` elements then has to be re-inserted
into an `a` of size ~`k`, costing O(k) rotations each. At n = 500 with disorder ≈ 0.19,
the LIS is long, so `k·(n−k)/4` lands in the same 10⁴ range as the current algorithm.
You trade one quadratic term for another.

Getting genuinely under 12 000 there needs **cost-based selection** — scanning `b` for
the cheapest element and sharing rotations with `rr`/`rrr`. That is the complex sorter.
Porting it into the low-disorder branch would make "simple" and "complex" the same
algorithm with different names, which undermines the subject's core requirement of four
*distinct* strategies.

**So the conclusion stands and it is a defensible one:**
- The subject *mandates* O(n²) below disorder 0.20. We obey it.
- The 12 000-operation target is specified for **random** input (VI.6), which has
  disorder ≈ 0.5 and routes to the O(n log n) strategy at ~5 090 operations.
- Any O(n²)-shaped algorithm lands in the 10⁴ range at n = 500. That is not a bug in
  our implementation; it is what the complexity class costs.

This is now written into the README under "Known trade-off, stated honestly", so you
are on record about it before an evaluator finds it. **Volunteer it — do not wait to
be asked.** The answer that scores is: *"the subject requires O(n²) there, and we chose
to respect the requirement rather than quietly route low-disorder input to a faster
class."*

### ✅ §4 Medium's chunk size — now derived from √n

**Before:** `chunk_size = 12` for n ≤ 100, `30` otherwise. Two constants fitted to the
two sizes the subject benchmarks. With a fixed constant `c` the true class is O(n²/c),
which is O(n²) — so the O(n√n) claim was not supported by the code.

**After:** `chunk_size = int_sqrt(a->size) * 3 / 2`, with `int_sqrt` added to
`algorithm_utils.c`. Now ~√n chunks of ~√n elements, and the complexity claim is
literal rather than fitted.

I measured three coefficients before picking one:

| chunk | n=100 | n=500 |
|-------|-------|-------|
| `1.0·√n` | 585 | 5244 |
| **`1.5·√n`** | **569** | **5075** |
| `2.0·√n` | 588 | 5246 |

`1.5·√n` won at both sizes and matches the old hand-tuned constants (574 / 5056)
within noise — so the claim got honest at no measurable cost. The 1.5 is a constant
factor and does not affect the complexity class; the README says so explicitly rather
than pretending it is pure √n.

### ⛔ §5 Complex sorter is O(n³) in CPU time — not changed, now documented

Not a subject violation: VI.3.1 is explicit that the complexity class must reflect
generated **operations**, not the runtime of the C code, and our operation count at
n = 500 is ~5 200 ≈ n·log₂n. Fixing it would mean caching targets and costs across
iterations — real work, real regression risk, and zero effect on your grade.

What changed is that it is no longer an undocumented surprise: there is an explicit
note in the README's complex-strategy section stating the difference between the
operation count and the runtime. If an evaluator asks about runtime, you have already
said it first.

### ✅ §6 Magic numbers → enum

`a->strategy == 1` in `main.c` and `benchmark.c` replaced with `SIMPLE` / `MEDIUM` /
`COMPLEX`. The old code only worked because the enum happened to be declared in that
order — reorder it and both files break silently. It also made `sort_dispatch`
unreadable to anyone who had not memorised the enum.

### ✅ §7 Duplicated include/comment — removed
See blocker 6.

### ✅ §8 Multi-byte characters in `write()` lengths — removed

`"Medium / O(n√n)"` with a hand-counted length of 18 (the `√` is 3 UTF-8 bytes) became
`"Medium / O(n*sqrt(n))"`, plain ASCII with a length anyone can count. The old byte
counts were all correct, but the next person to edit that string would have had to know
to count bytes rather than characters. Whole class of bug removed.

### ✅ §9 Multiple quoted strings — now accepted

**Before:** exactly one non-flag argument was split on spaces; two or more were each
treated as a single number. So `./push_swap "4 67 3" "87 23"` → `Error`.

**After:** every non-flag argument goes through `ft_split`, so `1 2 3`, `"1 2 3"` and
`"1 2" "3"` are all equivalent. The special case in `build_stack` disappeared and
`first_number()` in `flags.c` became dead code and was deleted.

**The behaviour that had to be preserved:** an argument that yields *no* token — an
empty or blank string — is still an error, so `./push_swap "" 1` → `Error`, matching
the subject's example. That is handled by the `if (!arr[0])` check in `parse_split`,
which hands the token array to `free_and_exit` so nothing leaks on that path.
Re-verified explicitly.

### ✅ §10 Dead file — deleted

`Code/tests/bench_main.c` (114 lines, entirely commented out) and `Code/.DS_Store`
removed.

---

## New files

| File | Why it exists |
|------|---------------|
| `Code/README.md` | Chapter VII |
| `Code/bench_utils.c` | `ft_putnbr_fd` + `ft_putpercent_fd`. `benchmark.c` would have had 6 functions after the SELECTED/EXECUTED split; the Norm allows 5 |
| `Code/stack_init.c` | `init_stack` + `reset_counters`. Adding the `resolved` field pushed `init_stack` to 26 lines; the Norm allows 25 |

Both splits were forced by the Norm, not by preference — worth knowing if you are asked
"why is this in its own file?".

---

## What to do before you push

1. **Read the Contributions table in the README** and correct it if it is wrong.
2. **Do not upload `checker_Mac`** to the school repo.
3. Run the demo script in `05_defense_prep.md` §3 once, end to end, so the new
   `--bench` format holds no surprises.
4. Re-read `05_defense_prep.md` §1 — the answers for the medium chunk size and the
   low-disorder trade-off both changed today, and both are now stronger.
