# Defense Prep

Both of you must be able to answer all of this. The subject (VI.1, IX) says either
learner can be asked about any part.

---

## 1. The questions you will definitely get

**"Show me your disorder metric."**
`disorder.c:15-40`. It is a literal transcription of the PDF pseudocode: nested loop
over all pairs, count inversions, divide by `n(n-1)/2`. Called from `main.c:99`,
after parsing and **before any operation is emitted**. Returns 0.0 for `size < 2`
(no pairs, so no division by zero).

**"Why those thresholds?"**
They are the subject's, not ours — VI.3.3.4 fixes `< 0.2`, `[0.2, 0.5)`, `≥ 0.5`.
What we chose is *which* algorithm fills each slot. Be ready to say that clearly:
the thresholds are given, the strategies are our design.

**"Prove your complex sorter is O(n log n)."**
Careful — answer in the right model. VI.3.1: complexity is measured in **generated
push_swap operations**, not CPU time. Turk's algorithm: each of the n elements is
moved from B to A exactly once, and each move costs at most the rotation distance,
which the cost function minimises by combining `rr`/`rrr`. Empirically n=500 →
~5100 ops ≈ 500 × log₂(500) ≈ 4500. State the empirical number; it is the honest
evidence.

Do **not** claim the C code runs in O(n log n) time — `assign_cost` is O(n²) per
iteration. If asked about *runtime*, say so plainly and point back to VI.3.1.

**"Prove medium is O(n√n)."**
*(Answer updated 2026-07-29 — this used to be the weakest claim, it no longer is.)*
The chunk size is now computed at runtime as `1.5 · √n` via `int_sqrt`, so there are
~√n chunks of ~√n elements and each element costs O(√n) rotations to reach its chunk
⇒ O(n√n). The 1.5 is a measured constant factor, not part of the class — say that
plainly; we tested 1.0, 1.5 and 2.0 and 1.5 won at both n=100 and n=500.

**"Run 500 numbers."**
```bash
ARG=$(python3 -c "import random;a=list(range(1,501));random.shuffle(a);print(' '.join(map(str,a)))")
./push_swap $ARG | wc -l          # ~5100
./push_swap $ARG | ./checker_Mac $ARG   # OK
```
5115 average over 10 runs — inside the < 5500 **excellent** band.

**"What if I give you an almost-sorted 500 list?"**
The honest answer, and you should volunteer it rather than be caught:
"Disorder drops below 0.2, so the subject requires the O(n²) branch, and we take it.
On such input we emit up to ~19 000 operations. The benchmark in VI.6 is specified
for *random* input, where we average 5115." If you implemented the improvement in
`04_code_findings.md` §3, say that instead — it's a much stronger answer.

**"Why a doubly-linked list?"**
`rra`/`rrb` need the tail in O(1). With a singly-linked list every reverse rotate
would be an O(n) walk. `t_stack` caches `head`, `tail`, `size`, `min`, `max`, so
`stack_o_meter` refreshes positions in a single O(n) pass.

**"How did you use AI?"**
Chapter III makes this a graded, explicit question, and the `Claude/` folder makes
the answer obvious — so answer it honestly and specifically in the README *and* out
loud. What actually happened is documented across `Claude/session_analysis_2026-07-01/`
and `Claude/defense_2026-07-14/`: design discussion, pseudocode, bug hunting, code
review. The rule that matters (Chapter III): **only use AI-generated content you
fully understand and can take responsibility for.** If there is any function either
of you cannot walk through line by line, that is the thing to fix this week — not
the README.

## 2. Complexity claims — the exact wording to use

| Strategy | Claim (in **operations**) | Justification |
|----------|---------------------------|---------------|
| Simple | O(n²) | Each of n elements requires a scan + up to n/2 rotations to find its insertion point ⇒ n × O(n) |
| Medium | O(n√n) | ~√n chunks × ~√n elements, each element costs O(√n) rotations within its chunk ⇒ n × O(√n) *(see caveat above about the constant chunk size)* |
| Complex | O(n log n) | Cost-minimising greedy with combined `rr`/`rrr`; measured 5115 ops at n=500 ≈ n log₂ n |
| Adaptive | Per regime, per VI.3.3.4 | Dispatches on the pre-measured disorder |

**Space:** O(n) for all four — n nodes, no auxiliary arrays. The only allocations are
`init_stack` (2 structs) and one `t_node` per input value. Say this; the subject asks
for a space bound too and it's the easiest point you'll score.

## 3. Live demo script

```bash
make re                                   # clean, no warnings
norminette *.c *.h                        # 18× OK

./push_swap                               # silent, prompt back
./push_swap 0 one 2 3                     # Error (stderr)
./push_swap --simple 3 2 3                # Error (duplicate)
./push_swap 2 1 3 6 5 8                   # 7 ops — subject's example does it in 12

ARG="4 67 3 87 23"
./push_swap --complex $ARG | ./checker_Mac $ARG    # OK
./push_swap --bench $ARG 2>&1 >/dev/null           # the metrics block

# the benchmark
ARG=$(python3 -c "import random;a=list(range(1,501));random.shuffle(a);print(' '.join(map(str,a)))")
./push_swap $ARG | wc -l
./push_swap $ARG | ./checker_Mac $ARG
```

Rehearse this. Have the 500-number generator ready in your shell history — fumbling
`shuf` on macOS in front of an evaluator is a bad look.

## 4. The "small live modification"

Chapter IX warns you may be asked to change something on the spot. For this project
the plausible asks, and where they land:

| Likely ask | Where you'd edit |
|------------|------------------|
| Add a new flag (e.g. `--quiet`) | `flags.c:55-70` + a `bool` in `t_stack` |
| Change an adaptive threshold | `main.c:55-60` (and mirror it in `benchmark.c:90-95` — **two places, don't forget**) |
| Change the bench output format | `benchmark.c` |
| Add a field to `t_node` and populate it | `push_swap.h:20-30` + `node_creator` + `stack_o_meter` |
| Print the op count to stdout instead of stderr | `benchmark.c:99-118`, change fd 2 → 1 |
| Make `three_sorter` handle 4 elements | `algorithm_utils.c:86-95` |

Note the trap in row 2: the disorder thresholds are **duplicated** in `main.c` and
`benchmark.c`. If you're asked to change one, change both, or the bench will report
a strategy you didn't run. Worth extracting into one helper *before* the defense.

## 5. Division of labour — decide this before you walk in

Git says `umutkilicaslan` has 42 commits and `ftessi` 19. An evaluator can run
`git shortlog -sne` in three seconds. That imbalance is fine *if* both of you can
explain everything — which is the actual requirement (VI.1.b, VI.1.d, IX). It is not
fine if one of you goes quiet on the complex sorter.

Agree in advance who explains what, but each of you should be able to take over any
section. The README must document both contributions (VI.1.c) — write that section
together and make it truthful.

## 6. Known weak points — own them before they're found

*Updated 2026-07-29 after the fixes in `06_changes_applied.md`.*

1. ~~No README~~ — **written**, at `Code/README.md`. Check the Contributions table
   is accurate; it is the one part that was inferred from git rather than known.
2. **Low-disorder n=500 exceeds 12000 ops** — still true, deliberately. This is now
   the only substantive weak point, and it is a defensible one. See §1 and the
   README's "Known trade-off, stated honestly". Volunteer it.
3. ~~Medium's chunk constant vs √n~~ — **fixed**, now `1.5 · √n` at runtime.
4. ~~`--bench` prints a strategy even when no algorithm ran~~ — **fixed**, it now
   prints `SELECTED` and `EXECUTED` separately.
5. ~~`--bench` disorder isn't a percentage~~ — **fixed**, prints `13.00%`.
6. **Complex sorter is O(n³) in CPU time** though its *operation* count is O(n log n).
   Not a violation (VI.3.1 measures operations), and now documented in the README.
   Don't demo n = 5000.
