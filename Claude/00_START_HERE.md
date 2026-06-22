# Push_swap — Walkthrough Index (START HERE)

> This folder is a **manual**, not a solution. It tells you *what* to build at each
> step and *why*, explains the concepts you need, and points you at the answer —
> but you and your teammate write every line of code. That is deliberate: the
> subject's "AI Instructions" chapter says you will fail the defense if you can't
> explain your own code. Use these files to *understand*, then implement.

## How to read this

The files are numbered in the order you should build the project. Each one:
- States the **goal** of the step.
- Explains the **concepts** you must understand first.
- Gives a **manual** (a guided path) to the solution, with questions to answer
  and pitfalls to avoid — but no finished C.
- Lists how to **verify** the step works before moving on.

## The big picture (what v1.1 actually asks for)

This is **not** the classic push_swap. Version 1.1 demands:

1. **Four distinct sorting strategies**, each in a stated complexity class
   (measured in *number of push_swap operations*, not array operations):
   - Simple — O(n²)
   - Medium — O(n√n)
   - Complex — O(n log n)
   - Adaptive — picks one of the above based on a measured *disorder* value.
2. A **disorder metric** (a number 0..1) computed *before* any moves.
3. A **runtime selector**: `--simple`, `--medium`, `--complex`, `--adaptive`
   (default). Any flag must work for any input.
4. A **benchmark mode** `--bench` printing stats to **stderr**.
5. Strict **error handling**, **Norm** compliance, **no leaks**, **no globals**,
   a non-relinking **Makefile**.
6. **Performance targets** (100 nums < 2000 ops, 500 nums < 12000 ops, etc.).
7. **Bonus**: a `checker` program.

## Build order (the files)

| File | Step | Why it comes here |
|------|------|-------------------|
| `01_understand_the_problem.md` | The rules, the 11 operations, what "sorted" means | You can't design before you understand the machine |
| `02_team_workflow.md` | How 2 people split this without colliding | Group project requirement; avoid merge pain |
| `03_project_setup.md` | Makefile, libft, file layout, Norm | Nothing compiles without this scaffold |
| `04_data_structure.md` | How to represent the stacks | Every later decision depends on this |
| `05_parsing_and_errors.md` | Validate args, build stack a | Garbage in = "Error"; do this before sorting |
| `06_operations.md` | Implement sa/sb/ss/pa/pb/ra/rb/rr/rra/rrb/rrr | The verbs your algorithms will speak |
| `07_disorder_metric.md` | The mandatory 0..1 disorder number | Required, and the adaptive strategy needs it |
| `08_strategy_simple.md` | O(n²) sort | The baseline, also used for tiny inputs |
| `09_strategy_medium.md` | O(n√n) chunk sort | The √n idea |
| `10_strategy_complex.md` | O(n log n) sort (radix recommended) | Hits the performance targets |
| `11_strategy_adaptive.md` | Disorder-driven dispatch | The "learner's design" piece |
| `12_cli_and_dispatch.md` | Parse flags, route to the right strategy | Ties strategies to the command line |
| `13_benchmark_mode.md` | `--bench` output to stderr | Required, and your tuning dashboard |
| `14_optimization.md` | Cut operation count (rr/rrr/ss, etc.) | To pass "good"/"excellent" thresholds |
| `15_bonus_checker.md` | The checker program | Only graded if mandatory is perfect |
| `16_testing.md` | How to prove it works, at scale | Defense and Deepthought both test this |
| `17_readme_and_defense.md` | README requirements + defense prep | Part of the grade; don't leave to the end |

## A note on honesty with yourselves

When a file says "figure out X," actually stop and figure it out with your
teammate at a whiteboard. The moments where you struggle are the moments you'll
be able to defend later. Read `01` next.
