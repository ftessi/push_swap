# What to do next — ordered plan

Your teammate built the **engine** (data structure + 11 operations + 3-sort). Your
job now is to make it a **real program** and then add the **strategies**. Do it in
this order — each step is testable before the next.

## Step 0 — Sync with your teammate (30 min)
This is a group project (`02_team_workflow.md`). Before touching anything:
- Agree who owns what. Natural split now: **you** take *parsing + main + CLI/dispatch*
  (steps 1–3, 7, 8); **teammate** continues *sorting strategies* (steps 4–6) since
  they wrote `three_sorter`.
- Agree the **sort direction is ascending** (the code already assumes it) and that
  `t_stack_check` is the stack handle everywhere.
- Commit the untracked `05b_parsing_readiness_review.md`; resolve the
  `split_psodo.txt` deletion. Start from a clean `git status`.

## Step 1 — Make `main` real (highest priority)
Turn `main(void)` into `main(int argc, char **argv)`. See `02_bugs_and_fixes.md` §2.
- `argc < 2` → exit 0, print nothing.
- Otherwise: parse → build stack A → if sorted, exit 0 → else dispatch.
- Move the current debug harness into a separate test file; don't ship it.
**Done when:** `./push_swap 3 1 2` reaches your code with a real stack A.

## Step 2 — Fix and wire the parser
- Fix `ft_atoi_ps` (the sign-skip, once-only sign, real overflow check — §1).
- Call, per token: `error_syntax` → `ft_atoi_ps` → `error_duplicate` →
  `node_creator` + `node_connecter`.
- Decide + document: do you support the quoted form `"3 1 2"` via `ft_split`? (You
  already have `ft_split`, so yes is cheap — split each argv on spaces.)
- Route every failure through `free_and_exit` (already correct: stderr + free + exit).
**Done when:** every row of the checklist in `02_bugs_and_fixes.md` passes, valgrind
clean on bad input.

## Step 3 — Norm pass
Run `norminette` and clear **every** file (tabs, no in-body comments, no ternaries,
no multi-statement lines, valid 42 headers). Do this now while the codebase is small.
**Done when:** `norminette *.c *.h` is all `OK`.

> After steps 1–3 you have a **correct, minimal push_swap** that sorts up to 3
> numbers and rejects bad input. That's a real milestone — commit it.

## Step 4 — Simple sort, any N (`08_strategy_simple.md`)
The O(n²) baseline that also handles the small cases (`sort_five`-style: push a
couple to B, sort A with the 3-sort, push back inserting in place). This makes the
program sort **arbitrary** input, not just 3.
**Done when:** random 5 and 100 numbers come out sorted (verify with a checker).

## Step 5 — Disorder metric (`07_disorder_metric.md`)
Re-add the `double disorder` computation (git history shows it was added and removed —
"disorder return back"). It's a single 0..1 number measured **once, before moving**.
It's mandatory in v1.1 and the adaptive strategy needs it.

## Step 6 — Medium & complex strategies (`09`, `10`)
- Medium: O(n√n) chunk sort.
- Complex: O(n log n), **radix** recommended — this is what hits the performance
  targets (100 < 700 ops, 500 < 5500 ops for "excellent").

## Step 7 — CLI flags + adaptive dispatch (`11`, `12`)
Parse `--simple/--medium/--complex/--adaptive` (default adaptive), route to the right
sort. Adaptive picks a strategy from the measured disorder.

## Step 8 — `--bench` mode (`13`) then optimize (`14`)
`--bench` prints stats to **stderr**. Then cut op counts (use `rr`/`rrr`/`ss`,
choose cheaper rotate direction, etc.) to clear the thresholds.

## Later — bonus checker (`15`), testing at scale (`16`), README + defense (`17`)

---

## Suggested next single session (2–3 h)
1. Step 0 sync (short).
2. Step 1 + Step 2: real `main` + fixed/wired parser. Test the whole checklist.
3. Step 3: norminette clean.
4. Commit: `"real main + working parser + norm clean"`.

That converts the current "engine on a bench" into a running program, and leaves the
teammate free to keep extending `three_sorter` into the simple sort. From there you
follow the numbered manual files `08 → 17` in order.
