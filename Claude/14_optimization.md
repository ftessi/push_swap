# Step 14 — Optimization (hitting the op-count targets)

> **Goal:** Reduce the number of emitted operations until you clear the benchmark
> thresholds. Correctness first, *then* this step. Never trade correctness for op
> count.

## The targets (from the subject)

| Input | Pass | Good | Excellent |
|-------|------|------|-----------|
| 100 numbers | < 2000 | < 1500 | < 700 |
| 500 numbers | < 12000 | < 8000 | < 5500 |

These are checked at evaluation. **Pass** is mandatory; aim for **excellent** if
you can — your `--complex` (radix) strategy realistically reaches it.

## The biggest lever: combined operations

`ss`, `rr`, `rrr` each cost **one** op but act on both stacks. Audit your code for
the anti-pattern of doing a move on `a` and the same-direction move on `b`
"around" the same moment:

- Emitting `ra` then `rb` → should be one `rr`.
- Emitting `rra` then `rrb` → should be one `rrr`.
- Emitting `sa` then `sb` → should be one `ss`.

This matters most in the **chunk** and **selection** strategies, where you often
need to rotate both stacks to align two targets. A classic optimization (especially
in chunk/insertion-style "cheapest move" sorts): when bringing one element up in
`a` and another in `b`, compute the rotations each needs and, while *both* need the
same direction, fire the combined op; finish the remainder with the single op.

> Do this only where it's genuinely simultaneous work. Don't contort an algorithm
> to force combined ops where they don't fit — measure with `--bench` and keep what
> helps.

## Lever 2: always rotate the cheaper direction

To bring a target to the top, you can `ra` (rotate up) or `rra` (rotate down). If
the target is in the **bottom half**, `rra` is fewer moves. Compute position vs
`size/2` and choose. Forgetting this is the most common reason a "correct"
algorithm has double the necessary ops. Applies to both stacks.

## Lever 3: tune the chunk size (medium strategy)

`√n` is the *asymptotic* sweet spot, but the best *constant* often comes from
slightly larger chunks. With your bench harness, sweep `cs` over a few values
(e.g. `√n`, `1.5√n`, `n/11`, fixed values like 11/13/16 for small n) on random 100
and 500 inputs and keep the best average. Document the value you chose and why.

## Lever 4: "cost function" upgrade (optional, for excellent on chunk-based)

If you went chunk/insertion instead of radix for your fast path, the well-known
optimization is: for each element in `b`, compute the **total cost** (rotations in
`a` to land it in the right spot + rotations in `b` to bring it up), pick the
element with the **minimum total cost**, exploit combined rotations, then `pa`.
This "cheapest insertion" approach is what gets chunk-based solutions under the
excellent line. Radix usually gets there more simply.

## Lever 5: skip work on near-sorted data

- If the stack is already sorted, emit nothing.
- The adaptive path already routes near-sorted inputs to the cheap method — make
  sure that method genuinely does few ops when there's little disorder (selection
  sort emits few moves when most elements are already in place only if you skip
  no-op moves — verify you never emit a rotate that accomplishes nothing).

## Don't-break-it rules while optimizing

- After **every** optimization, re-run the checker on many random inputs. A
  micro-optimization that desyncs a combined op will silently corrupt sorts.
- Keep a "known good, slower" version in git history so you can bisect a
  regression.
- Re-check valgrind — optimizations sometimes introduce leaks or out-of-bounds.

## Measuring properly

Use a loop (Step 16 has the script) that runs e.g. 100 random trials of 100 and
500 numbers, pipes each through the checker (must be OK), and records the op count
distribution (min / avg / max). **Max** is what matters for "pass" — one bad seed
over the threshold fails you. Tune until even the worst case clears.

## Verify
- 500-number worst case over many trials is under your target tier.
- 100-number worst case under target tier.
- Every trial still checks OK.
- valgrind clean at n=500.

➡ Next: `15_bonus_checker.md`
