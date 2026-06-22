# Step 17 — README & Defense Preparation

> **Goal:** Write the required `README.md` (it's graded) and prepare both of you to
> defend *any* part. Don't leave this to the last hour — write the algorithm
> rationale while it's fresh.

## README requirements (from the subject — treat as a checklist)

The `README.md` at the repo root **must** include:

1. **First line, italicized, exactly:**
   `*This project has been created as part of the 42 curriculum by <login1>, <login2>.*`
   (Both of your logins — group project.)
2. **Description** section: what the project is, its goal, brief overview.
3. **Instructions** section: how to compile (`make`, `make bonus`), install, run;
   usage examples with the flags.
4. **Resources** section: classic references on the topic (sorting, complexity,
   radix sort, etc.) **and** a description of **how AI was used** — which tasks,
   which parts. Be honest and specific (e.g. "used AI to clarify radix-on-stacks
   intuition and to review parser edge cases; all code written and understood by
   us"). The subject explicitly asks for this.
5. **Both learners' contributions** clearly documented (who did what).
6. **Detailed explanation and justification of the algorithms** selected — this is
   required and overlaps with the adaptive-strategy documentation below.

English is recommended (or your campus's main language).

## The algorithm documentation the subject demands

For the **adaptive** strategy you must document:
- **Your thresholds** (0.2 and 0.5 are given) and the **rationale** for why those
  boundaries make sense in terms of operation cost.
- The **internal technique used in each regime** (which of your three algorithms,
  and why it fits that disorder band).
- A brief **complexity argument (upper bounds)** for **time and space**, stated in
  the **push_swap operation model** (i.e. counting emitted ops, not array ops).

Also briefly justify each of the three base strategies: why selection-style is your
O(n²), why √n chunks is O(n√n), why radix is O(n log n) in this model.

Write a short table:
```
Regime            Disorder     Strategy used   Op-count bound   Space
Low (nearly sorted) d < 0.2     Simple          O(n^2)           O(1) extra / O(n)
Medium              0.2<=d<0.5  Chunk (sqrt n)  O(n*sqrt(n))     O(n)
High (random)       d >= 0.5    Radix (LSD)     O(n log n)       O(n)
```

## Defense preparation (both must do this)

The subject is blunt: **both** of you must explain **any** part. Expect:
- "Walk me through how `rrr` is implemented." → know the mutation cold.
- "Why √n chunks?" → the `n·k + n²/k` balance argument (Step 09).
- "Prove your radix sorts." → stability + LSD ordering (Step 10).
- "Why O(n²) for low disorder instead of always radix?" → fewer real ops on
  near-sorted data; constant factors (Step 11).
- "Show me no leaks." → run valgrind live.
- "Change this behavior in 5 minutes." → the subject warns of a live modification
  (e.g. tweak a display, add a field, change a threshold). Practice small edits.

### Quiz each other
Sit down and have your teammate ask you to derive the 3-element optimal sequences,
explain the disorder formula, and trace one bit-pass of radix on paper. If you
stumble, that's the part to re-study — better now than at the table.

## Pre-submission final checklist

- [ ] `make`, `make bonus`, `make re`, `make clean`, `make fclean` all behave; no
      relink.
- [ ] No globals; only allowed external functions used; your own `ft_printf`.
- [ ] Norm clean on every file (mandatory + bonus).
- [ ] valgrind clean on normal runs **and** error paths, mandatory + bonus.
- [ ] All four strategies present, selectable, correct; adaptive default.
- [ ] `--bench` to stderr with exact fields; ops stream clean on stdout.
- [ ] Disorder matches the subject's sample (`40.00%` for `4 67 3 87 23`).
- [ ] Benchmarks pass for 100 and 500 in worst case over many trials.
- [ ] Error handling exactly matches the spec (stderr, no leaks, no crash).
- [ ] README complete with both logins, contributions, algorithm justification,
      AI-usage note.
- [ ] Both logins are real Git contributors; both present at defense.

## One last reminder
The bonus is only evaluated if the mandatory part is **perfect** — all benchmarks
pass, no errors. Make mandatory flawless before polishing the checker.

Good luck. Sort smart, not slow.
