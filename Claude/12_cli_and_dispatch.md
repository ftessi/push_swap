# Step 12 — CLI Parsing & Strategy Dispatch

> **Goal:** Turn the command line into (1) a validated stack and (2) a decision
> about which strategy and which output mode to run. Flags and numbers are mixed in
> `argv`; sort out the routing cleanly.

## The flags you must support

| Flag | Effect |
|------|--------|
| `--simple`   | force O(n²) strategy |
| `--medium`   | force O(n√n) strategy |
| `--complex`  | force O(n log n) strategy |
| `--adaptive` | force adaptive (this is also the **default** when no selector given) |
| `--bench`    | turn on benchmark output to **stderr** (combinable with any selector) |

Key rules from the subject:
- **Any selector must work for any input** — size and disorder independent.
- **No selector → adaptive.**
- `--bench` is *orthogonal* to the selector: you can have `--bench --adaptive`,
  `--bench --complex`, etc. (see the usage examples).
- **No parameters at all → print nothing, exit 0.**

## Design: separate "what" from "how"

Three independent pieces of state come out of parsing:
1. The **numbers** → stack `a` (Step 05).
2. The **strategy** → an enum (default ADAPTIVE).
3. The **bench flag** → 0/1.

Keep these decoupled. A clean parse loop classifies each `argv[i]`:
- starts with `--` and matches a known flag → set the corresponding state.
- starts with `--` but unknown → error (or document treating it as invalid).
- otherwise → it's a number, validate & add to stack.

Watch for **conflicts**: two selectors at once (`--simple --complex`)? Decide a
policy — last-one-wins, or error. Document whichever you pick. Simplest defensible
choice: last selector wins; `--bench` is a separate boolean.

## The top-level flow (main)

Think of `main` as a thin conductor:

```
parse argv  -> fills ctx (stack a, strategy, bench) or errors out
if stack empty (no numbers):        # no params
    return 0                        # print nothing
if already sorted:
    (optionally) skip sorting       # emit nothing; bench still reports
compute disorder (before any moves) # Step 07
run selected strategy               # Steps 08-11
if bench: print bench report to stderr   # Step 13
free everything
return 0
```

Keep `main` short (Norm!). Push the real work into `parse`, `dispatch`,
`run_strategy`, `print_bench`.

## The dispatch function

A small function mapping the strategy enum → the right sort call:
```
switch (ctx->strategy):
    SIMPLE   -> sort_simple(ctx)
    MEDIUM   -> sort_medium(ctx)
    COMPLEX  -> sort_complex(ctx)
    ADAPTIVE -> sort_adaptive(ctx)   # which itself re-dispatches by disorder
```
Each strategy, when it runs, should also stamp into `ctx` the **label** and
**complexity string** it represents, so `--bench` prints the truth. For adaptive,
the *delegated* strategy stamps it (matching the subject's `Adaptive / O(n√n)`
style output — note it shows "Adaptive" as the chosen-strategy *name* with the
delegated complexity class).

## Edge cases to handle deliberately

- **Already sorted input with a forced flag:** `./push_swap --complex 1 2 3` should
  emit nothing (it's sorted) but, under `--bench`, still report disorder 0.00 and
  total_ops 0. Don't run a sort that emits spurious ops on sorted data.
- **Single number / empty:** print nothing; with `--bench` report disorder 0.00.
- **Flag but no numbers:** `./push_swap --simple` → no numbers → print nothing,
  exit 0 (no error — there's just nothing to sort).
- **`--bench` placement:** it can appear anywhere among args; don't assume
  position.

## Manual
1. Define the strategy enum and add `strategy` + `bench` to your context.
2. Write the classify-and-route parse loop (reuse number validation from Step 05).
3. Write `dispatch`/`run_strategy`.
4. Wire `main` as the thin conductor above.
5. Test every flag and the no-flag default.

## Verify
```
./push_swap --simple 5 4 3 2 1     | ./checker 5 4 3 2 1   # OK
./push_swap --medium $ARG          | ./checker $ARG        # OK
./push_swap --complex $ARG         | ./checker $ARG        # OK
./push_swap $ARG                   | ./checker $ARG        # OK (adaptive default)
./push_swap                                              # nothing, exit 0
./push_swap --adaptive 0 one 2 3                          # Error (stderr)
```

➡ Next: `13_benchmark_mode.md`
