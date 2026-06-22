# Step 11 — Strategy 4: Adaptive (the learner's design)

> **Goal:** A strategy that **measures disorder first** and then dispatches to a
> method whose complexity matches the regime. This is the default behavior, and
> the subject requires you to **document your reasoning** in the README.

## The contract (non-negotiable thresholds)

The subject fixes the complexity you must use per regime:

| Disorder | Required complexity | Reach for... |
|----------|--------------------|--------------|
| `< 0.2`  | **O(n²)**          | your Simple strategy |
| `0.2 ≤ d < 0.5` | **O(n√n)**  | your Medium strategy |
| `≥ 0.5`  | **O(n log n)**     | your Complex strategy |

Note the *inversion of intuition*: when the input is **nearly sorted** (low
disorder), you use the **O(n²)** method — not because it's asymptotically better,
but because on almost-sorted data it does very *few actual operations* (few
inversions to fix), and it avoids the overhead/constant factors of radix. When the
input is **very messy** (high disorder), the O(n log n) method wins decisively.
This is the whole pedagogical point of the project: *the best algorithm depends on
the input, not just on Big-O.*

## What "adaptive" must do, concretely

1. Read `ctx->disorder` (already computed in Step 07, before any moves).
2. Compare against the two thresholds.
3. Call the matching already-written strategy function.
4. Record **which** method it chose and its complexity class, because `--bench`
   must report the strategy name + complexity actually used.

That's it — adaptive is mostly *routing*. The intelligence is in the three
strategies you already built. Don't reinvent a fourth algorithm; *compose*.

## But you're allowed (and encouraged) to be smarter

The subject says the internal techniques are "entirely up to you" as long as you
respect the complexity per regime. Reasonable enhancements to mention/justify:

- **Small-n short-circuit:** if `n ≤ 5`, just call your hardcoded small-sort
  regardless of disorder — it's optimal and the thresholds don't meaningfully
  apply. (Document this as an explicit design choice.)
- **Already sorted:** if disorder is exactly 0, emit nothing. Cheap win.
- **Threshold rationale:** you must explain *why* 0.2 and 0.5. You don't get to
  change the numbers (they're given), but you must argue why those boundaries make
  sense — e.g. "below 0.2 the input is close enough to sorted that inversion-bound
  insertion-style work beats radix's fixed n·log n cost; above 0.5 the input is
  effectively random and only n log n stays within the op budget."

## Manual

1. Write `sort_adaptive(ctx)`:
   - (optional) handle `n ≤ 5` and `disorder == 0` early.
   - branch on the two thresholds.
   - call `sort_simple` / `sort_medium` / `sort_complex` accordingly.
   - store the chosen strategy label + complexity string in `ctx` for `--bench`.
2. Make it the **default** when no selector flag is given (Step 12).
3. Write the README paragraph **now** while the reasoning is fresh: your
   thresholds, the technique used in each regime, and a one-line upper-bound
   complexity argument (time and space) in the push_swap model. The subject
   *explicitly grades this documentation.*

## Pitfalls
- **Recomputing disorder after moves.** It must be measured on the *original*
  stack. Compute once, store, reuse.
- **Boundary handling.** Is `0.2` medium or simple? The subject uses `< 0.2` for
  simple and `0.2 ≤ d` for medium — so exactly 0.2 is **medium**. Match the
  inequalities precisely.
- **Bench mismatch:** the strategy name/complexity you print in `--bench` must be
  the one actually executed, including when adaptive delegates. (See the subject's
  bench example: it shows `Adaptive / O(n√n)` — the *delegated* complexity.)
- **Forgetting the README justification** — easy points lost.

## Verify
- Feed a nearly-sorted input → adaptive picks simple (check via `--bench` strategy
  line).
- Feed a random 500 → adaptive picks complex, passes the op target.
- Feed a moderately shuffled input straddling 0.2–0.5 → picks medium.
- Each `--simple/--medium/--complex` flag still forces its own method regardless of
  disorder (adaptive logic must not leak into the forced flags).

➡ Next: `12_cli_and_dispatch.md`
