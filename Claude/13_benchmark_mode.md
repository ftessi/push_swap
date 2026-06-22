# Step 13 — Benchmark Mode (`--bench`)

> **Goal:** When `--bench` is present, after sorting, print a stats block to
> **stderr** (never stdout). This is required *and* it's your single most useful
> tuning tool — you'll stare at it constantly during optimization (Step 14).

## Exactly what to print (from the subject)

To **stderr**, only when `--bench` is set, the block contains:
- The computed **disorder** as a percentage with **two decimals** (e.g. `49.93%`).
- The **strategy name** used and its **theoretical complexity class**
  (e.g. `Adaptive / O(n√n)`).
- The **total number of operations**.
- The **count of each operation type**: `sa sb ss pa pb ra rb rr rra rrb rrr`.

The subject's example format (match it closely):
```
[bench] disorder:  49.93%
[bench] strategy:  Adaptive / O(n√n)
[bench] total_ops: 7997
[bench] sa: 0  sb: 0  ss: 0  pa: 500  pb: 500
[bench] ra: 4840  rb: 1098  rr: 0  rra: 0  rrb: 1059  rrr: 0
```

The operation stream stays on **stdout**; the bench block goes to **stderr**. This
separation is what lets `./push_swap --bench $ARG 2> bench.txt | ./checker $ARG`
work — operations flow to the checker, stats divert to the file.

## Why stdout/stderr separation is the whole point

If you accidentally print bench lines to stdout, the checker will read them as
"operations," fail to parse them, and report `Error`/`KO`. The graders pipe
push_swap into the checker; mixing streams breaks that pipe. Use **fd 2** for every
bench line. Confirm with `2>/dev/null` (bench disappears) and `1>/dev/null` (only
bench remains).

## You already did the hard part

Because each operation function increments `ctx->op_count[OP]` (Step 06), the
counts are *already collected* by the time sorting finishes. Bench mode is just
**formatting** that table plus the disorder you stored in Step 07 plus the strategy
label the dispatcher stamped in Step 12. No recomputation, no second sort.

## Concepts / gotchas

- **Two-decimal percentage:** disorder is a `double` in `[0,1]`. Print
  `disorder * 100` with two decimals. If your `ft_printf` doesn't do floats/width,
  you must either extend it or format the number yourself (integer part, then two
  fractional digits via `*100` rounding). **`printf` from libc is not allowed** —
  only your own `ft_printf`/equivalent and `write`. Plan for float formatting in
  your printf, or build a small `put_percent` helper using integer math.
- **total_ops** = sum of the 11 counters. Note `ss`/`rr`/`rrr` each count as 1 (not
  2), consistent with the cost model.
- **Strategy label string** must reflect what actually ran. For `--complex` it's
  `Complex / O(n log n)`; for adaptive it's `Adaptive / O(...)` with the delegated
  class. Keep these strings in one place.
- **Order of operations:** sort first (which fills counts), *then* print bench. If
  you print before sorting, all counts are zero.

## Manual
1. Add an `op_count[11]` table + a label/complexity field to `ctx` (done in Step
   04/06 if you followed along).
2. Write `print_bench(ctx)` that emits the block to fd 2 using your own output
   functions. Centralize the per-op count printing.
3. Ensure float/percentage formatting works — test it in isolation.
4. Call `print_bench` from `main` only when `ctx->bench` is set, after sorting.

## Verify
```
./push_swap --bench --adaptive 4 67 3 87 23 2>&1 1>/dev/null
# should show: disorder 40.00%, strategy line, total_ops, per-op counts

./push_swap --bench $ARG 2> bench.txt | ./checker $ARG
# checker prints OK; bench.txt holds the stats; the pipe is clean
```
Cross-check `total_ops` against `./push_swap $ARG | wc -l` — they must match.

➡ Next: `14_optimization.md`
