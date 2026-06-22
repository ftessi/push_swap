# Step 07 — The Disorder Metric (mandatory)

> **Goal:** Compute a single number in `[0, 1]` describing how unsorted stack `a`
> is, **before any moves**. This is a hard requirement *and* the input to your
> adaptive strategy (Step 11).

## What the subject specifies

- `0` = already sorted ascending. `1` = worst possible order (fully reversed).
- Method: look at **all pairs** `(i, j)` with `i < j`. Each pair where the earlier
  element is **bigger** than the later one is a **mistake** (an *inversion*).
- disorder = `mistakes / total_pairs`.
- The subject gives the exact pseudocode — implement it faithfully:

```
mistakes = 0; total_pairs = 0
for i in 0..n-1:
    for j in i+1..n-1:
        total_pairs += 1
        if a[i] > a[j]: mistakes += 1
return mistakes / total_pairs
```

- **Measure it before doing any moves.** Compute it right after parsing, store it
  in your context, and never recompute on a mutated stack.

## Concepts behind it

This is the **number of inversions** divided by the maximum possible number of
inversions.

- Maximum pairs for `n` elements is `n*(n-1)/2` — that's `total_pairs`. (Notice
  this is also why naive computation is **O(n²)**; for n ≤ 500 that's ~125k
  comparisons — instant. Don't over-engineer it.)
- A fully sorted ascending array has 0 inversions → disorder 0.
- A fully reversed array has *every* pair inverted → disorder 1.
- This is a well-known measure of "sortedness"; insertion sort's running time is
  proportional to the number of inversions, which is exactly why the *simple*
  algorithm is appropriate when disorder is low (few inversions to fix).

## Things to be careful about

- **Integer division.** `mistakes / total_pairs` with both as ints gives 0. Cast
  to `double` (or float) for the ratio. Store it as a `double`.
- **n = 0 or 1.** `total_pairs` is 0 → division by zero. Guard it: define disorder
  as 0 when there are fewer than 2 elements (nothing can be out of order).
- **Compute on a stable snapshot.** Index access `a[i]` is easy if you have an
  array; with a linked list, either copy values into a temp array first (cleanest
  for this O(n²) double loop) or walk carefully. Copying to a temp `int` array for
  this one computation is perfectly reasonable — just free it.
- **Raw values vs ranks.** Inversions are identical whether you compare raw values
  or their ranks (ranking preserves order). Use whichever you have handy.

## Why it matters beyond "it's required"

The adaptive strategy reads this number to pick an algorithm:
- `disorder < 0.2` → use the **O(n²)** method (input is nearly sorted; cheap
  methods win and avoid overhead).
- `0.2 ≤ disorder < 0.5` → **O(n√n)**.
- `disorder ≥ 0.5` → **O(n log n)**.

So the metric isn't decoration — it's the steering wheel. Get it exact.

## Manual

1. Snapshot `a`'s values (array of ints) without modifying the real stack.
2. Run the double loop exactly as the subject's pseudocode.
3. Return/store a `double`. Guard n < 2 → 0.0.
4. In `--bench` you'll print it as a percentage with two decimals (Step 13), e.g.
   `49.93%`. Keep the raw `double`; format only at print time.

## Verify (use the bench examples as ground truth)
- Sorted input `1 2 3 4 5` → disorder `0.00`.
- Reversed `5 4 3 2 1` → disorder `1.00`.
- The subject's bench example: `4 67 3 87 23` → `40.00%`. Hand-check it: list the
  10 pairs, count inversions, divide by 10. If you don't get exactly 0.40, your
  loop bounds or comparison direction is wrong.

➡ Next: `08_strategy_simple.md`
