# Step 09 — Strategy 2: Medium, O(n√n)

> **Goal:** A sorter whose operation count grows like `n·√n`. The canonical fit is
> **chunk-based sorting** with about `√n` chunks. This is the conceptual bridge
> between the naive O(n²) and the clever O(n log n).

## The core idea: chunks

Sorting one element at a time into the right place is O(n²). Sorting in *fully
sorted order immediately* is hard. The middle ground: process the values in
**bands** (chunks). You don't care about exact order *within* the band while
pushing, only that the band's values get parked in `b` together; you fix order
when pushing back.

### Why √n chunks gives O(n√n)
- You split the value range `0 .. n-1` into `k` chunks, each holding ~`n/k`
  values.
- To push all of `a` into `b`, you rotate through `a` once per chunk-ish: total
  pushing cost is on the order of `n` rotations per chunk × `k` chunks... but with
  the "park roughly, order later" trick the dominant cost balances to **O(n·k)**
  for pushing plus **O(n·(n/k))** for the within-chunk ordering when bringing back.
- Total ≈ `n·k + n²/k`. Minimize over `k`: the two terms balance when `k ≈ √n`,
  giving **O(n√n)**. That's the whole reason the magic number is √n. Be ready to
  explain this trade-off at defense — it's the heart of this strategy.

## How chunk sort actually runs

Work in **ranks** (`0 .. n-1`), not raw values — this is what makes "which chunk
does this belong to?" trivial.

Phase 1 — **push to b in chunks:**
- Define chunk size `cs ≈ n / √n = √n`. So chunk 0 = ranks `[0, cs)`, chunk 1 =
  `[cs, 2cs)`, etc.
- Rotate through `a`. For each element on top:
  - If its rank belongs to the **current target chunk** (lowest unprocessed
    chunk), `pb` it. Optionally `rb` after pushing small-valued items so larger
    values within b stay roughly ordered (a common refinement that keeps the
    final `pa` cheap).
  - Otherwise `ra` to skip it (you'll come back).
- Advance chunk by chunk until `a` is empty (or until only a few remain that you
  finish with your small-sort).

Phase 2 — **push back from b to a, largest first:**
- `b` now holds all values; because you parked by chunk, the **maximum** is always
  near the top of `b`. Find the current max in `b`, rotate `b` the short way to
  bring it to the top, `pa`. Repeat.
- Since you push the largest remaining each time, `a` ends sorted ascending with
  smallest on top.

A common refinement: while pushing in phase 1, if the value is in the *lower part*
of the current chunk, do `pb` then `rb` (sink it), so phase 2 finds things more
ordered and rotates less.

## Concepts you must nail

- **Mapping rank → chunk index:** `chunk = rank / cs`. Choosing `cs` is your knob;
  `cs ≈ √n` is the target, but tune it (Step 14) — slightly larger chunks often
  reduce real op counts.
- **"Short way" rotation:** to bring a target to the top, compare its position to
  size/2 and use `ra`/`rra` (or `rb`/`rrb`) accordingly. Critical for keeping the
  constant factor down.
- **Finding the max in b efficiently:** you'll do it O(n) times; each scan is O(n).
  That's fine within the O(n√n) budget, but cache positions if you can.

## Manual

1. Compute ranks for all elements (you may already do this from Step 04).
2. Implement "rotate stack X to bring rank R to top using the cheaper direction."
   You'll reuse this everywhere.
3. Implement phase 1 (chunked `pb`) with a tunable `cs`.
4. Implement phase 2 (repeatedly bring max of b to top, `pa`).
5. Test correctness first with a generous `cs`, *then* tune toward `√n` and measure
   op counts.

## Pitfalls
- **Treating this as quicksort.** Chunking is range-partitioning by value, not
  pivoting. Keep it simple and band-based.
- **Forgetting the cheaper rotation direction** — doubles your ops.
- **Not advancing chunks correctly** → infinite loop or leftover elements.
- **Claiming O(n√n) but writing O(n²).** Make sure your design actually balances
  the two cost terms; if you scan the whole stack for *every single element*, you
  slipped back to n². Process chunk-by-chunk.

## Verify
- Sorts 100 and 500 correctly (checker → OK).
- Operation count clearly **between** your simple and complex strategies, and
  scales sub-quadratically as n grows (double n → ops grow by ~2.8×, not 4×).

➡ Next: `10_strategy_complex.md`
