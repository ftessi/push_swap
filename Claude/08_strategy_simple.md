# Step 08 — Strategy 1: Simple, O(n²)

> **Goal:** A correct baseline sorter in the O(n²) operation class. It must work
> for **any** input size (the `--simple` flag has to sort 500 numbers too, just
> slowly). It also doubles as your "tiny input" specialist.

## Why start here

It's the easiest to get *correct*, and correctness on small inputs (n ≤ 5) is
reused by your better algorithms. If your fancy radix sort bottoms out by pushing
the last 3 elements back, it needs a rock-solid 3-element sort. Build that here.

## Concept: what O(n²) means *in this model*

Remember the cost is counted in **push_swap operations**, not array comparisons.
An O(n²) strategy is one where the number of emitted operations grows roughly with
n². Selection-style and insertion-style approaches both land here.

## Pick a baseline approach (any of these qualify)

### Approach 1 — Selection sort via push/rotate (recommended, simple to reason about)
Idea: repeatedly find the **smallest remaining** element in `a` and get it onto
`b`, until `a` is empty; then push everything back.

More precisely, one common form:
- While `a` has elements: find the position of the current minimum. Rotate `a`
  (using `ra` or `rra`, whichever is fewer moves — the element is in the top or
  bottom half) until that min is on top, then `pb`.
- When `a` is empty, `b` holds the values in descending order from top, so `pa`
  everything back gives ascending in `a`.

Why it's O(n²): each of the n elements costs up to O(n) rotations to bring to the
top.

### Approach 2 — Insertion-ish / bubble-ish via `sa` + `ra`
Repeatedly scan: if top two are out of order, `sa`; otherwise `ra`. After enough
full passes the stack is sorted. Easy to write, easy to defend, clearly O(n²).

### Approach 3 — Min-extraction (the subject lists it explicitly)
Same spirit as Approach 1.

> Choose **one** and implement it cleanly. Approach 1 generally produces far fewer
> ops than Approach 2, so prefer it unless you want maximum simplicity.

## The small-n cases you should special-case

These are worth hardcoding because they're used a *lot* (by the adaptive and
complex strategies as a finishing step):

- **n = 1:** already sorted, emit nothing.
- **n = 2:** if out of order, `sa`. Done.
- **n = 3:** there are only 6 permutations. Work out the **optimal** sequence for
  each by hand (each needs at most 2 operations using `sa`, `ra`, `rra`). This
  little routine is gold — write it once, reuse it.
- **n = 4 and n = 5:** push the smallest one or two to `b`, sort the remaining 3
  with your n=3 routine, push back, then place the parked elements. Aim for a small
  bounded op count (a good 5-sort lands well under 12 ops).

Encapsulate these as `sort_three`, `sort_small`, etc. They're the building blocks
everything else leans on.

## Manual

1. Write `sort_two`, `sort_three` (hand-derive the 6 cases), then `sort_small` for
   4–5 using `sort_three` as a subroutine.
2. Write the general selection sort for arbitrary n (this is the "real" `--simple`
   algorithm).
3. Make `--simple` dispatch route to: tiny cases → the hardcoded routines; larger
   → the selection sort.
4. Verify each emitted op actually moves you toward sorted (use the checker once
   it exists, or your debug printer + `is_sorted` at the end).

## Pitfalls
- **Choosing the wrong rotation direction.** If the target is in the bottom half
  of the stack, `rra` reaches it in fewer moves than `ra`. Decide by comparing the
  element's position to size/2. Forgetting this roughly doubles your op count
  (still O(n²), but ugly).
- **Re-finding min by value vs by rank.** Be consistent; ranks make "is this the
  current minimum?" trivial.
- **Off-by-one when `a` empties.** Stop conditions on push-everything-back loops.

## Verify
- `./push_swap --simple 5 4 3 2 1` sorts correctly (pipe to your checker → OK).
- `sort_three` is optimal for all 6 permutations (≤ 2 ops each).
- Works on 100 and 500 (slowly, many ops — that's expected for O(n²)).

➡ Next: `09_strategy_medium.md`
