# Push_swap — Defense Documentation (2026-07-14)

> **Purpose:** a complete, line-by-line walkthrough of the *actual* final code, so
> either teammate can defend any part of the project in front of an evaluator.
> Every file in `Code/` is covered. Read `07` before your defense — it lists the
> questions an evaluator will ask and the risks you must be ready to explain.

## How to read this folder

| Doc | Covers | Files explained |
|-----|--------|-----------------|
| `01_data_structures.md` | The types everything is built on | `push_swap.h` |
| `02_parsing_pipeline.md` | argv → validated stack | `flags.c`, `parser.c`, `build_stack.c`, `ft_split.c` |
| `03_stack_and_operations.md` | The linked list + the 11 moves | `stack_utils.c`, `ops_push.c`, `ops_swap.c`, `ops_rotate.c`, `ops_rev_rotate.c` |
| `04_metrics_disorder_targets.md` | Measuring & targeting | `disorder.c`, `algorithm_utils.c` |
| `05_algorithms.md` | The three sorters | `simple.c`, `medium.c`, `complex.c`, `complex_utils.c` |
| `06_main_and_dispatch.md` | Program lifecycle + routing | `main.c` |
| `07_defense_qa_and_risks.md` | Q&A, complexity, known issues | — |

## The program in one paragraph

`push_swap` receives a list of integers and must print the **shortest possible
list of stack operations** that, when applied to stack A, sort it in ascending
order (smallest on top). We model A and B as **doubly-linked lists**. We first
**parse & validate** the input, then **measure how disordered** it is, then pick a
**sorting strategy** sized to that disorder, and finally emit the operations to
stdout. A custom `--bench` flag prints a stats block so we could compare strategies
during tuning.

## The pipeline (data flow)

```
argv
  │  parse_flags()      → set strategy/bench, count number args   (flags.c)
  │  build_stack()      → validate each number, build stack A      (build_stack.c + parser.c + ft_split.c)
  ▼
stack A (doubly-linked list of t_node)
  │  compute_disorder() → % of out-of-order pairs                  (disorder.c)
  │  is_sorted()        → early-out if already sorted              (stack_utils.c)
  ▼
sort_dispatch()  → choose simple / medium / complex by disorder    (main.c)
  │  each strategy emits ops via sa/sb/pa/pb/ra/... to STDOUT       (ops_*.c)
  ▼
sorted stack A, operation list printed, memory freed
  │  if --bench: print_benchmark_summary() to stdout               (main.c)
```

## Build & run

```sh
cd Code
make                       # produces ./push_swap
./push_swap 3 2 1          # prints the ops that sort 3 2 1
./push_swap "3 2 1"        # same, single quoted-string form
./push_swap --bench 5 4 3  # prints ops + a stats block
# Grader-style check (needs the 42 checker binary):
ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_OS $ARG   # expect OK
```

## The 11 operations (the only allowed "moves")

| Op | Meaning |
|----|---------|
| `sa` / `sb` / `ss` | swap top two of A / B / both |
| `pa` / `pb` | push top of B→A / top of A→B |
| `ra` / `rb` / `rr` | rotate A / B / both up (top goes to bottom) |
| `rra` / `rrb` / `rrr` | reverse-rotate A / B / both down (bottom goes to top) |

## ⚠️ Three things to know before you defend (full detail in `07`)

1. **The algorithm flags don't actually pick the algorithm.** `sort_dispatch`
   routes purely by measured **disorder**; it never reads `a->strategy`. So
   `--simple`/`--medium`/`--complex` are parsed but ignored for selection. Only
   `--bench` has a visible effect. Be honest about this if asked.
2. **`printf` / `<stdio.h>` are used** (the `--bench` summary in `main.c`). These
   are **not in the allowed functions** for the mandatory part. Fine for your own
   testing, but must be stripped/guarded for the official evaluation.
3. **Comments sit inside function bodies.** Norminette forbids that. Run
   `norminette` and clean up before the graded defense — the comments here are for
   *your* understanding, not for submission.
