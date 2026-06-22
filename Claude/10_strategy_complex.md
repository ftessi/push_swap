# Step 10 — Strategy 3: Complex, O(n log n)

> **Goal:** The fast one. This is the strategy that must comfortably hit the
> benchmark targets (500 nums < 5500 ops for "excellent"). **Pair-program this.**
> Recommended: **Radix sort (LSD) on ranks.** Alternatives: merge/quick adaptation.

## Why radix is the sweet spot here

Once values are converted to **ranks `0 .. n-1`**, every element is an integer you
can write in binary with `B = ceil(log2(n))` bits. Radix sort orders numbers by
processing one bit at a time. With two stacks this is astonishingly clean and its
operation count is **O(n · B) = O(n log n)** — exactly the target.

For n = 500, `B = 9` bits. Each bit pass moves every element once or twice, so the
total is on the order of `2·n·B ≈ 2·500·9 = 9000` worst case, and with the cheap
tricks it lands well under 5500 in practice. That's why radix is the go-to.

## How LSD radix sort works on two stacks

You start with ranks in `a`. Repeat for each bit position `i` from 0 (least
significant) up to `B-1`:

1. Look at each of the n elements currently in `a` (iterate n times):
   - If **bit `i` of its rank is 0**, `pb` (send it to `b`).
   - If **bit `i` of its rank is 1**, `ra` (keep it in `a`, rotate past it).
2. After processing all n, everything with a 0 bit is in `b` (in order) and the
   1-bits stayed in `a`. Now `pa` everything back: push all of `b` back onto `a`.

After bit `i`, the elements are sorted by the low `i+1` bits. After the most
significant bit pass, `a` is fully sorted (smallest on top, because rank 0 is the
smallest).

> **Why this sorts:** radix sort is *stable* and processes least-significant bit
> first, so each pass refines the order established by previous passes. Make sure
> you can articulate "stability + LSD ordering ⇒ correct" at defense.

## The bit-counting that gives B

- Find `max_rank = n - 1`.
- `B` = number of bits needed to represent `max_rank` = smallest `B` with
  `(max_rank >> B) == 0`. Compute it in a tiny loop. Don't hardcode 9 — it must
  scale to 100, 500, or whatever.

## Critical correctness details

- **Sort on ranks, not raw values.** Radix on raw ints with negatives/huge ranges
  is a nightmare. Ranks make bits meaningful and bounded.
- **Each bit pass is exactly n iterations of "look at top, decide pb or ra."**
  After the loop, push *all* of `b` back with `pa`. Don't interleave wrongly.
- **Number of passes is B**, not n. If you loop the wrong bound you either
  under-sort (wrong result) or waste ops.
- **Smallest on top:** since rank 0 is smallest and LSD radix ends ascending,
  verify the final orientation matches "smallest at top." If reversed, you mapped
  ranks backwards — fix the rank assignment, not the radix.

## Alternatives (if you want to defend something other than radix)

- **Quicksort with stack partitioning:** choose a pivot (e.g. median rank),
  partition `a` into `b` (below pivot) and keep above in `a`, recurse. Conceptually
  elegant but the operation accounting and recursion-with-two-stacks bookkeeping is
  trickier than radix. More ops in practice.
- **Merge sort on two stacks:** doable but fiddly; radix usually beats it on op
  count and on simplicity. Only pick this if your team genuinely prefers it.

> Recommendation: **implement radix.** It's the shortest path to passing the
> "excellent" threshold and the easiest O(n log n) to explain.

## Manual

1. Confirm your ranks are correct (`0 .. n-1`, rank 0 = smallest).
2. Write `count_bits(max_rank)` → `B`.
3. Write the bit-pass loop: for `i` in `0..B-1`: n× (check bit i of top's rank →
   `pb` or `ra`), then `pa` until `b` empty.
4. Run on small inputs and watch it sort with your debug printer before trusting it
   on 500.

## Pitfalls
- **Reading the rank's bit, not the value's bit.** `(rank >> i) & 1`.
- **Forgetting to push b back each pass**, or pushing back inside the n-loop.
- **Mis-counting B** → off-by-one leaves it unsorted by the top bit.
- **Counting `pb`+`pa` cost honestly** — each is one op; the bench must reflect it.

## Verify
- `./push_swap --complex 4 67 3 87 23 | ./checker $ARG` → OK.
- 100 random: target < 700 ops for "excellent" — radix typically hits this.
- 500 random: target < 5500 ops for "excellent" — radix typically hits this.
- Doubling n grows ops by ~2× plus a bit (the extra bit), confirming n log n.

➡ Next: `11_strategy_adaptive.md`
