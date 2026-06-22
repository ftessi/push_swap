# Step 16 — Testing & Validation

> **Goal:** Prove the project is correct and fast *at scale* and *on edge cases*,
> automatically, so a passing defense isn't luck. Build these test helpers early
> and run them after every change. (Test programs aren't submitted/graded, but you
> may use them at defense.)

## The two questions every test must answer

1. **Does it sort?** Pipe push_swap's output through a checker on the same input;
   it must say `OK`. The checker is your oracle — use the provided `checker_linux`
   and/or your bonus checker.
2. **Is it within budget?** `push_swap $ARG | wc -l` must be under the threshold
   for that size.

## Core correctness loop (build this script)

A shell loop you can run hundreds of times:
```
ARG=$(shuf -i 1-100000 -n 100 | tr '\n' ' ')
RES=$(./push_swap $ARG | ./checker $ARG)
N=$(./push_swap $ARG | wc -l)
echo "$RES  ops=$N"
```
Wrap it in a `for` loop of, say, 200 iterations. Track: every line must be `OK`,
and collect the **max** ops. One `KO` = a real bug. One run over threshold = fail
risk.

## Edge cases to test explicitly (not just random)

- `./push_swap` (no args) → nothing, exit 0.
- `./push_swap 42` → nothing.
- `./push_swap 2 1` → `sa`.
- Already sorted `1 2 3 ... 100` → 0 ops.
- Reverse sorted `100 99 ... 1` → still under budget (this is the disorder=1 worst
  case for the adaptive path; make sure complex handles it well).
- Duplicates, non-ints, overflow, empty string, `+`/`-` → `Error` to stderr.
- Each flag: `--simple --medium --complex --adaptive`, and `--bench` combos.
- Very small with flags: `./push_swap --complex 2 1`.

## Performance measurement (the table you must beat)

Run many trials at n=100 and n=500 (and ideally n=2,3,5 for the small path), record
min/avg/**max** ops per strategy. The **max** is what evaluation cares about. Make
a tiny script that prints something like:
```
n=100  adaptive  min=.. avg=.. max=..   (target pass<2000 good<1500 excellent<700)
n=500  adaptive  min=.. avg=.. max=..   (target pass<12000 good<8000 excellent<5500)
```
Do the same for `--complex` explicitly (it should be your best).

## Memory & robustness

- `valgrind --leak-check=full ./push_swap $ARG > /dev/null` → **0 leaks, 0 errors**
  — including on the **error paths** (`valgrind ./push_swap 1 2 two`).
- Same for the checker.
- No segfault / bus error / double free on any input. Fuzz with weird args.

## Compilation hygiene

- `make` twice → no relink the second time.
- Recompiles cleanly from `make re` with `-Wall -Wextra -Werror`, **no warnings**.
- `norminette` clean on **all** files including bonus.
- `make fclean` removes all artifacts (objects, binaries, libft objects).

## A sanity check against the subject's own numbers

The subject shows `4 67 3 87 23` → disorder `40.00%`, 13 ops under adaptive. Run
exactly that and confirm your disorder and (roughly) op count line up. Confirm
`5 4 3 2 1 --simple` and `2 1 3 6 5 8` produce valid sorting sequences.

## Keep a test log

For defense, keep a short record: "500-number adaptive: max 5xxx ops over 1000
trials, all OK, valgrind clean." Being able to *show* this is persuasive and fast.

➡ Next: `17_readme_and_defense.md`
