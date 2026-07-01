# Session Analysis — 2026-07-01

> Written after inspecting `repo/Code/` at commit `a79b6c7` ("disorder return back").
> You (Francisco) were away a few days; your teammate (umutkilicaslan) pushed the
> work described here. This folder is the full analysis for this session:
>
> - `00_README.md` — this file: where the project stands + what to do next.
> - `01_code_walkthrough.md` — your teammate's code explained **line by line**.
> - `02_bugs_and_fixes.md` — the concrete bugs, ranked, with how to fix each.
> - `03_next_steps.md` — the ordered plan for your next work sessions.

---

## TL;DR — where the project is

**Good news:** the project **compiles clean** (`-Wall -Wextra -Werror`) and the
low-level machine is basically done and mostly correct:

- The **data structure** is a doubly linked list with a container struct
  (`t_stack_check` holding `head`/`tail`/`min`/`max`/`size`). Solid choice.
- All **11 operations** are implemented: `sa sb ss / pa pb / ra rb rr / rra rrb rrr`.
  The pointer surgery in swap/push/rotate/rev-rotate looks **correct**.
- `ft_split`, `is_sorted`, `stack_o_meter` (the min/max/position scanner),
  `free_stack`, and node creation/append are all present and look correct.
- `three_sorter` (the 3-element sort) is implemented and **I traced all 6
  permutations — it is correct** for an **ascending** sort.

**The catch:** it doesn't work as a real `push_swap` yet, and there are real bugs:

1. **`main()` is a hardcoded test harness.** It takes `void`, not
   `(int argc, char **argv)`, and never reads the command line. It builds three
   fixed numbers and prints debug tables. So `./push_swap 3 1 2` ignores its args.
2. **The parser is written but never called**, and **`ft_atoi_ps` is broken**
   (see below). `error_syntax` / `error_duplicate` are also never wired in.
3. **Norminette fails in every file** — mostly spaces-instead-of-tabs, comments
   inside function bodies, ternaries, and lines with multiple statements. This is
   graded; it will cost points and block the defense.
4. Only the **3-element sort** exists. The subject (v1.1) wants **four strategies**
   (simple / medium / complex / adaptive), a **disorder metric**, a **flag
   selector**, and **`--bench`**. None of those exist yet.

Bottom line: **the engine is ~80% built and correct; the car has no driver.** The
next job is to connect parsing → main → a real sort, then build the strategies.

See `02_bugs_and_fixes.md` for the bug details and `03_next_steps.md` for the plan.

---

## What "done" looks like (from the subject + `00_START_HERE.md`)

| Piece | Status now |
|-------|-----------|
| Data structure (doubly linked list) | ✅ done, correct |
| 11 operations | ✅ done, look correct |
| `ft_split` | ✅ done |
| Parsing + error handling | 🟡 written but not wired, `ft_atoi_ps` broken |
| `main(argc, argv)` real entry point | ❌ still a test harness |
| Disorder metric (0..1 `double`) | ❌ removed again (see git log "disorder return back") |
| Simple sort O(n²) | ❌ |
| Medium sort O(n√n) | ❌ |
| Complex sort O(n log n) (radix) | ❌ |
| Adaptive dispatch | ❌ |
| Flag selector `--simple/--medium/--complex/--adaptive` | ❌ |
| `--bench` mode | ❌ |
| Norminette clean | ❌ fails in every file |
| No leaks (valgrind on good + bad input) | ⚠️ untested (harness frees, but error path untested) |
| Bonus `checker` | ❌ |

---

## The one-paragraph "what should I do next"

Talk to your teammate so you don't collide (this is a group project — see
`02_team_workflow.md`). Then: **(1)** turn `main` into a real
`main(int argc, char **argv)` that calls the parser and, for now, runs
`three_sorter` when `size == 3`; **(2)** fix `ft_atoi_ps` (it's genuinely broken —
it returns wrong numbers) and wire in `error_syntax`/`error_duplicate`; **(3)** run
`norminette` and fix every file (tabs, no in-function comments). Once
`./push_swap 3 1 2` prints `ra` (or a valid short sequence) and `./push_swap 1 a`
prints `Error`, you have a real, if tiny, push_swap. **Then** build the
simple sort (for any N), and only after that the medium/complex/adaptive
strategies. Details and order in `03_next_steps.md`.
