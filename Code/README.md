*This project has been created as part of the 42 curriculum by ftessi, ukilicas.*

# push_swap

## Description

`push_swap` sorts a list of integers using **two stacks** and a **restricted set of
11 operations**, and prints the sequence of operations that performs the sort. The
program never sorts "in place" for display — it computes a program in the Push_swap
language and writes it to standard output, one instruction per line.

The point of the project is not that sorting is hard. It is that sorting *cheaply*,
when your only moves are swap / push / rotate / reverse-rotate, forces you to think
in terms of **algorithmic complexity measured in operations**, not in CPU cycles.

The two stacks are `a` (holds the input, must end sorted ascending with the smallest
value on top) and `b` (starts empty, used as scratch space). The available operations
are `sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`.

This implementation ships **four distinct strategies** in one binary and picks one at
runtime based on a **disorder metric** measured before any move is made.

## Instructions

### Build

```bash
make          # build ./push_swap
make clean    # remove object files
make fclean   # remove object files and the binary
make re       # fclean + build
```

Compiled with `cc -Wall -Wextra -Werror`. No external library is used beyond
`read`, `write`, `malloc`, `free` and `exit`; every helper (`ft_split`, the integer
parser, the number printers) is written from scratch in this repository.

### Run

```bash
./push_swap 2 1 3 6 5 8            # numbers as separate arguments
./push_swap "2 1 3 6 5 8"          # or as one quoted string
./push_swap "2 1 3" "6 5 8"        # or any mix of the two
```

Every non-flag argument is split on spaces, so all three forms are equivalent.
The **first number is the top of stack a**.

### Strategy selection

| Flag | Forces | Complexity class (in operations) |
|------|--------|----------------------------------|
| `--simple` | Insertion/selection baseline | O(n²) |
| `--medium` | Chunked distribution | O(n·√n) |
| `--complex` | Cost-based greedy (Turk) | O(n log n) |
| `--adaptive` | Picks one of the above from the disorder | per regime, see below |

`--adaptive` is the **default** when no selector is given. Flags may appear anywhere
in the argument list; if several strategy flags are given, the last one wins.

### Benchmark mode

```bash
./push_swap --bench 5 3 9 1 7            # operations on stdout, metrics on stderr
./push_swap --bench $ARG 2>&1 >/dev/null # metrics only
./push_swap --bench $ARG 2>bench.txt | ./checker_linux $ARG   # check + save metrics
```

`--bench` writes to **stderr only**, so the operation stream on stdout stays clean
and can still be piped into a checker. It reports:

```
 ================== OPS SUMMARY ==================
SWAPS    | SA : 0	| SB : 0	| SS : 0
PUSHES   | PA : 2	| PB : 2
ROTATES  | RA : 3	| RB : 0	| RR : 0
REV-ROT  | RRA: 2	| RRB: 0	| RRR: 0
--------------------------------------------------
METRICS  | Initial Disorder: 50.00%
         | SELECTED : Adaptive -> Complex / O(n log n)
         | EXECUTED : Complex / O(n log n)
--------------------------------------------------
 TOTAL MOVES: 9
==================================================
```

`SELECTED` is what the flags and the disorder asked for. `EXECUTED` is what actually
generated the operations — the two differ when the input was already sorted (nothing
runs) or had only 2-3 elements (a hardcoded micro-sort beats any general strategy).

### Errors

`Error` followed by a newline is written to **stderr**, and the program exits with
status 1, when an argument is not an integer, does not fit in an `int`, or duplicates
another value. With no arguments at all the program prints nothing and exits 0.

```bash
./push_swap                  # (nothing, exit 0)
./push_swap --adaptive 0 one 2 3   # Error
./push_swap --simple 3 2 3         # Error
./push_swap 2147483648             # Error
```

## The disorder metric

Disorder is a number in `[0, 1]` measuring how far stack `a` is from being sorted.
It is the fraction of pairs that are inverted:

```
disorder = (number of pairs i < j with a[i] > a[j]) / (total number of pairs)
```

`0` means already sorted, `1` means reversed. It is computed in `disorder.c` with the
double loop from the subject, over the doubly-linked list, and it is measured
**before any operation is emitted** (`main.c`, right after parsing).

Cost: O(n²) time, O(1) extra space. That is affordable because it runs once, and for
n = 500 it is ~125 000 comparisons — invisible next to the sort itself.

## Algorithms and why we chose them

All complexity claims below are stated in the **number of push_swap operations
generated**, as the subject requires — not in the runtime of the C code. Where the
two differ we say so explicitly.

### 1. Simple — insertion into a sorted stack B — O(n²)

*Files: `algo_simple.c`*

Push elements from `a` into `b` one at a time, keeping `b` **sorted descending** at
all times: for each new element we find its insertion point in `b` (`target_the_b`)
and rotate `b` until that point is on top. When only 3 elements remain in `a` we sort
them with the hardcoded `three_sorter`, then pull everything back from `b`, again
finding each element's landing spot in `a` and rotating to it. A final rotation puts
the minimum on top.

**Why this one:** it is the honest O(n²) baseline the subject asks for, and it is the
easiest to reason about — it is an insertion sort where "insert" costs a rotation
instead of a memory shift. Both the outbound and the return phase cost one scan plus
up to n/2 rotations per element, so **O(n²) operations, O(n) space**.

We deliberately did *not* optimise this one. It exists to show the cost of a naive
approach: at n = 500 it emits ~32 500 operations where the complex strategy emits
~5 200. That contrast is the whole point of the project.

### 2. Medium — chunked distribution — O(n·√n)

*Files: `algo_medium.c`*

First, `assign_rank` replaces every value with its **rank** (0 … n-1) so the algorithm
is immune to the actual magnitudes (negatives, huge gaps, `INT_MIN`). Then we sweep
`a` and push elements to `b` in **chunks** of consecutive ranks:

- rank inside the chunk already passed → `pb` then `rb` (send it to the bottom of `b`)
- rank inside the current chunk window → `pb` (keep it near the top of `b`)
- otherwise → `ra` (skip it, look at the next one)

This keeps `b` roughly ordered by chunk without ever paying for a full insertion. The
3 largest elements stay in `a` and are sorted by `three_sorter`. Recovery is then
simple: repeatedly bring the maximum of `b` to the top and `pa` it, which builds `a`
in perfect ascending order with no final rotation needed.

**Chunk size is `1.5 · √n`**, derived from the input size at runtime — that is what
makes the complexity class real rather than a constant fitted to the test sizes. With
~√n chunks of ~√n elements, each element costs O(√n) rotations to reach its chunk, so
**O(n·√n) operations, O(n) space**. The 1.5 factor is a measured constant (it beat 1.0
and 2.0 at both n = 100 and n = 500); it does not change the complexity class.

**Why this one:** chunking is the natural middle ground. It gives up the per-element
optimality of the greedy strategy in exchange for never scanning the whole stack, and
in practice it is within a few percent of the O(n log n) strategy at these sizes while
being far easier to explain and to prove.

### 3. Complex — cost-based greedy (Turk) — O(n log n)

*Files: `algo_complex.c`, `algo_complex_utils.c`*

Push everything to `b` except 3 elements, sort those 3, then repeat until `b` is empty:

1. For every node in `b`, find its **target** in `a` — the smallest value in `a` that
   is still larger than it (wrapping to the minimum when none is larger). That is the
   position where it can be inserted without breaking `a`'s order.
2. Compute the **signed rotation distance** for the node in `b` and for its target in
   `a`. `signed_rot` returns a positive count for `ra`/`rb` and a negative one for
   `rra`/`rrb`, choosing the shorter direction.
3. The **cost** of a node is the number of operations needed to align both stacks. When
   both distances point the same way, the shared rotations are done with `rr`/`rrr` and
   the cost is the *larger* of the two, not the sum — this is where the savings come
   from.
4. Move the cheapest node, and repeat.

A final rotation puts the minimum on top.

**Why this one:** it is the classic optimal-ish push_swap strategy and it is the only
one of the three that actively exploits `rr`/`rrr`. Each of the n elements is moved
back exactly once, and the greedy choice keeps the per-move rotation cost low; measured
at n = 500 it emits ~5 200 operations, against n·log₂n ≈ 4 500. **O(n log n) operations,
O(n) space.**

> **Note on runtime vs. operations.** `assign_cost` re-scans both stacks on every
> iteration, so the *C code* runs in O(n³) time even though the *operation count* is
> O(n log n). The subject is explicit that the complexity class must reflect generated
> operations, so the claim above is the right one — but we are aware of the difference.
> At n = 500 it is 0.01 s; at n = 5000 it becomes slow.

### 4. Adaptive — the default

*Files: `main.c` (`resolve_strategy`)*

The adaptive strategy measures the disorder once, then dispatches:

| Regime | Condition | Method chosen | Required class | Our class |
|--------|-----------|---------------|----------------|-----------|
| Low | `disorder < 0.20` | Simple (insertion into sorted `b`) | O(n²) | O(n²) ✔ |
| Medium | `0.20 ≤ disorder < 0.50` | Chunked distribution | O(n·√n) | O(n·√n) ✔ |
| High | `disorder ≥ 0.50` | Cost-based greedy | O(n log n) | O(n log n) ✔ |

Stacks of 2 or 3 elements bypass this entirely and use a hardcoded micro-sort (1 or 2
operations); no general strategy can do better, and running one would only waste moves.
An already-sorted stack emits nothing at all.

**Rationale for the thresholds.** The three cut-offs are the ones fixed by the subject.
What we chose is *which* technique fills each slot, and the reasoning is that the cost
of a strategy should match how much work the input actually needs:

- **Below 0.20** the stack is nearly sorted. Most elements are already in relative
  order, so an insertion-style method finds short rotation distances and the quadratic
  worst case is never reached in practice — at very low disorder it emits an order of
  magnitude fewer operations than on random input. Paying for the bookkeeping of a
  smarter algorithm would not earn it back.
- **Between 0.20 and 0.50** the input has real structure but is not random. Chunking
  exploits that structure: elements land near their neighbours without a global scan.
- **At or above 0.50** there is no structure left to exploit, so we spend the extra
  analysis and use the cost-based greedy, which is the only one of the three that pays
  off on genuinely random input.

`resolve_strategy` is the single place these thresholds exist. `--bench` calls the same
function to report `SELECTED`, so the reported strategy and the executed one can never
drift apart.

**Space, all four strategies: O(n).** We allocate one `t_node` per input value and two
`t_stack` structures. There is no auxiliary array, no recursion on the input size, and
nothing is copied — the algorithms only relink existing nodes.

### Known trade-off, stated honestly

The adaptive rule is required to use an O(n²) method below disorder 0.20. At n = 500
with a disorder in roughly the 0.09 – 0.20 band, that costs more than 12 000
operations. This does not affect the subject's benchmark, which is specified on
**random** input (disorder ≈ 0.5, which routes to the O(n log n) strategy and averages
~5 100 operations). We chose to respect the subject's complexity requirement rather
than quietly route low-disorder input to a faster class, because the requirement is the
exercise.

## Performance

Measured against the provided checker, random permutations, default (`--adaptive`)
selection. Averages over 12-20 runs.

| Input | Requirement | Good | Excellent | **Measured** |
|-------|-------------|------|-----------|--------------|
| 100 random | < 2000 | < 1500 | < 700 | **574** (533-617) |
| 500 random | < 12000 | < 8000 | < 5500 | **5090** (4892-5479) |

Per-strategy, same inputs:

| Strategy | n = 100 | n = 500 |
|----------|---------|---------|
| `--simple` | 1414 | 32514 |
| `--medium` | 564 | 5060 |
| `--complex` | 573 | 5207 |
| `--adaptive` (default) | 574 | 5090 |

Correctness was verified exhaustively for every permutation of n = 1…7 against all
five selection modes (~28 000 sorts, zero failures), plus randomised runs with negative
and non-contiguous values.

## Project structure

| File | Role |
|------|------|
| `push_swap.h` | Types (`t_node`, `t_stack`, `t_strategy`) and all prototypes |
| `main.c` | Entry point, `resolve_strategy`, dispatch, cleanup |
| `flags.c` | `--simple/--medium/--complex/--adaptive/--bench` parsing |
| `parser.c` | Integer syntax, range and duplicate validation, error exit |
| `build_stack.c` | argv → validated stack `a` |
| `stack_init.c` | `t_stack` allocation and initial state |
| `ft_split.c` | Splits a quoted argument on spaces |
| `stack_utils.c` | Node creation/linking, positions, min/max, `is_sorted`, free |
| `disorder.c` | The disorder metric |
| `ops_push.c`, `ops_swap.c`, `ops_rotate.c`, `ops_rev_rotate.c` | The 11 operations |
| `algorithm_utils.c` | Target lookup, 3-element sort, integer square root |
| `algo_simple.c` | O(n²) strategy |
| `algo_medium.c` | O(n·√n) strategy |
| `algo_complex.c`, `algo_complex_utils.c` | O(n log n) strategy |
| `benchmark.c`, `bench_utils.c` | `--bench` report |

Data structure: a **doubly-linked list** with cached `head`, `tail`, `size`, `min` and
`max`. The second link is what makes `rra`/`rrb` O(1) instead of a full walk, and the
cached extremes let `stack_o_meter` refresh every node's position in a single pass.

## Contributions

This is a two-person project and it was built that way: both of us worked across the
whole codebase rather than splitting it into private halves, and the git history shows
both authors on essentially every file. Both of us can explain any part of it.

| Area | Lead | Reviewer |
|------|------|----------|
| Stack structure and the 11 operations | ukilicas | ftessi |
| Parsing, validation and error handling | shared | shared |
| Disorder metric | ftessi | ukilicas |
| Simple strategy | ukilicas | ftessi |
| Medium strategy | ukilicas | ftessi |
| Complex strategy | ftessi | ukilicas |
| Flags, dispatch, benchmark mode | shared | shared |

## Resources

- Donald Knuth, *The Art of Computer Programming*, Vol. 3 — sorting and the origin of
  the Big-O framing the subject opens with.
- [Big-O cheat sheet](https://www.bigocheatsheet.com/) — complexity classes reference.
- [Sorting algorithms visualised](https://www.toptal.com/developers/sorting-algorithms)
  — useful for seeing why a chunked pass behaves differently from an insertion pass.
- The "Turk" algorithm, the community name for the cost-based greedy strategy in
  push_swap; we reimplemented it from the description of its principle (target lookup,
  signed rotation distance, `rr`/`rrr` sharing) rather than from existing code.
- 42 subject `Push Swap.pdf` v1.1 — the specification this README answers to.

### Use of AI

Per the subject's AI instructions, here is what AI (Claude) was used for and what it
was not:

**Used for:**
- **Planning.** Breaking the project into an ordered set of milestones (data structure
  → operations → parsing → metric → strategies → benchmark) before writing code.
- **Algorithm discussion.** Working through the principle of the cost-based greedy
  strategy and of chunked distribution, and writing pseudocode we then implemented
  ourselves.
- **Debugging.** Diagnosing a segmentation fault in the simple strategy and a set of
  parsing bugs, by describing the symptom and reasoning about the cause.
- **Refactoring advice.** Extracting flag handling into its own module, and splitting
  functions to satisfy the Norm.
- **Review and audit.** A pre-delivery pass over the whole project against the subject:
  norm, leaks, benchmark targets, and error handling. Several fixes in this repository
  came out of that review — the percentage formatting of the disorder in `--bench`,
  deriving the medium chunk size from `√n` instead of a hardcoded constant, freeing the
  stack structure on error paths, and reporting the executed strategy separately from
  the selected one.
- **Documentation.** Drafting this README and the in-file summary comments.

**Not used for:** none of the sorting logic was accepted without being understood.
Where AI proposed something we could not explain, we either reworked it or dropped it —
the `--simple` strategy, for example, was deliberately left unoptimised because its
purpose is to demonstrate the cost of a naive approach.

Everything in this repository is code we can read line by line and defend.
