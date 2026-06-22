# Step 01 — Understand the Problem

> **Goal:** Be able to explain, without notes, what push_swap is, what the two
> stacks are, what each of the 11 operations does, and what "the output" actually
> is. If you skip this, every later step will feel like guessing.

## What the program *is*

`push_swap` does **not** sort numbers and print them. It prints a **list of
instructions** that, if executed on stack `a`, would sort it. You are writing a
program that *thinks about sorting* and emits a recipe. The numbers are given as
command-line arguments; the **first argument is the top of the stack**.

```
./push_swap 2 1 3 6 5 8
```
Here the top of stack `a` is `2`, then `1`, then `3`, ... The program prints
operations (one per line, separated only by `\n`) to **stdout**.

## The two stacks

- **Stack `a`**: starts full, in the given order. Goal: end **sorted ascending**,
  smallest on top.
- **Stack `b`**: starts **empty**. It is scratch space — a place to park numbers.

"Stack" means LIFO: you mostly touch the **top**. But two of the operations
(`rra`, `rrb`) reach the **bottom**, so it behaves more like a double-ended
structure. Keep that in mind when you choose your data structure (Step 04).

## The 11 operations (memorize these cold)

Think of each stack drawn vertically, **top at the top**.

| Op | Meaning | No-op when |
|----|---------|-----------|
| `sa` | swap top two of `a` | a has < 2 elements |
| `sb` | swap top two of `b` | b has < 2 elements |
| `ss` | `sa` **and** `sb` together | — |
| `pa` | pop top of `b`, push onto `a` | b is empty |
| `pb` | pop top of `a`, push onto `b` | a is empty |
| `ra` | rotate `a` up: top goes to bottom | — |
| `rb` | rotate `b` up: top goes to bottom | — |
| `rr` | `ra` **and** `rb` together | — |
| `rra` | reverse rotate `a`: bottom goes to top | — |
| `rrb` | reverse rotate `b`: bottom goes to top | — |
| `rrr` | `rra` **and** `rrb` together | — |

### Critical insight about the combined ops
`ss`, `rr`, `rrr` each count as **one** operation but do work on both stacks.
This is your main lever for hitting the performance targets later. If your
algorithm ever does `ra` then `rb` back-to-back, that should have been one `rr`.

## Work the example by hand

The subject sorts `2 1 3 6 5 8` in 12 ops. **Do not read past this until you have
replayed it on paper.** Draw both stacks, apply `sa`, then `pb pb pb`, then `rr`,
then `rrr`, then `sa`, then `pa pa pa`, and confirm `a` ends as `1 2 3 5 6 8`.

If your hand-trace doesn't match, your mental model of an operation is wrong —
find which one. This pencil exercise is the single best investment in the project.

## Questions you must be able to answer before continuing

1. After `pb`, where does the moved element sit on `b` — top or bottom?
2. If `a = [3,1,2]` (3 on top), what is `a` after `ra`? After `rra`?
3. Why is `rr` strictly better than `ra` + `rb` even though both "rotate both"?
4. The goal is "smallest on top." Is that ascending from top to bottom? (Yes —
   make sure you and your teammate agree on which end is "top" in your code.)

## What "the smallest list of operations" means in practice

The subject says "display the smallest list of operations possible." Don't read
this as "provably optimal" — that's intractable for large n. Read it as: **be
efficient enough to pass the benchmark thresholds** (Step 16). Your job is good
algorithms, not a mathematical proof of minimality.

➡ Next: `02_team_workflow.md`
