# Blockers Before Delivery

Ordered by how much they cost you. Nothing here touches the sorting logic — the
algorithms are finished.

---

## 🔴 1. There is no README.md

Chapter VII is **mandatory** and it is currently 0% done. It is also where the
subject makes you justify the adaptive thresholds (VI.3.3.4) and document both
learners' contributions (VI.1, IX).

Required sections, verbatim from the subject:

- **First line, italicised, exactly this shape:**
  `*This project has been created as part of the 42 curriculum by ftessi, umutkilicaslan.*`
- **Description** — goal + brief overview.
- **Instructions** — compilation, installation, execution.
- **Resources** — classic references, **plus a description of how AI was used,
  for which tasks and which parts of the project.** (Chapter III makes this
  non-negotiable; `Claude/` shows AI was used heavily, so say so honestly and
  specifically.)
- **Detailed explanation and justification of the algorithms selected.**
- **Adaptive rationale** — your thresholds, the internal technique per regime, and
  an upper-bound complexity argument for time *and space* in the push_swap model.
- **Both learners' contributions.**

Raw material for all of this already exists in `Claude/defense_2026-07-14/` and in
`04_code_findings.md` / `05_defense_prep.md` of this folder. It is a writing job,
not a research job.

## 🔴 2. The sources are not where the evaluator will look

Today:
```
repo/Code/Makefile   repo/Code/*.c   repo/Code/*.h
```
The evaluator clones, runs `make` at the root, and gets `No targets specified`.

**Fix — move the project to the repository root:**
```bash
cd /Users/franciscojtessi/Dev/push_swap/repo
git mv Code/Makefile Code/*.c Code/push_swap.h .
```

While you are there, clean the tracked junk:
```bash
git rm --cached Code/.DS_Store
git rm Code/tests/bench_main.c      # 100% commented out, dead
git rm psodo.txt                     # scratch pseudocode
# checker_Mac: school binary, not your work — remove or keep out of the graded tree
git rm --cached Code/checker_Mac
```

Add a `.gitignore`:
```
.DS_Store
*.o
push_swap
```

⚠️ Do this on a branch and confirm `make re && norminette *.c *.h` still passes
before you push. Also confirm `Claude/` and `Push Swap.pdf` stay **untracked** —
they currently are, and they should stay that way.

## 🟠 3. `--bench` disorder is not a percentage

Subject VI.5: the benchmark must display *"The computed disorder (**% with two
decimals**)"*.

Current output:
```
METRICS  | Initial Disorder: 0.13
```
Expected shape:
```
METRICS  | Initial Disorder: 13.00%
```

This is not pedantry — the current form also **destroys precision at low disorder**:
a stack with disorder 0.003 prints `0.00`, which looks like a bug during a demo.

**Fix** — `benchmark.c:111`, multiply by 100 and add the sign:

```c
ft_putdouble_fd(a->disorder * 100.0, 2);
write(2, "%", 1);
```

`ft_putdouble_fd` already rounds to two decimals correctly; no other change needed.

## 🟠 4. Makefile uses `gcc`, subject says `cc`

Chapter II: *"compiles your source files to the required output with the flags
-Wall, -Wextra, and -Werror, **using cc**."*

`Makefile:15` → `CC = gcc`. On macOS these are the same binary, so nothing breaks,
but it is a literal, one-word deviation from a written requirement and evaluators do
grep for it.

```make
CC = cc
```

Also tidy `Makefile:21-22` — the `SRC` list ends with a backslash on a blank
continuation line. It works, but it reads as an accident.

## 🟡 5. `free_and_exit` leaks the stack struct

`parser.c:103-121` frees every node via `free_stack(stack)` but never frees the
`t_stack` allocation itself. Valgrind reports this as *still reachable* (~120 bytes),
not *definitely lost*, so most evaluators will not fail you on it — but `main.c`
already does it correctly in `clean_exit`, so the inconsistency is the only reason
it is still there.

```c
	if (stack)
	{
		free_stack(stack);
		free(stack);
	}
	write(2, "Error\n", 6);
	exit(1);
```

Watch the call sites: `build_stack.c` passes `a` and then returns to `main`, but
`free_and_exit` always `exit()`s, so there is no use-after-free risk. Verify with
`make re && ./push_swap 1 2 three` after the change.

## 🟡 6. Duplicated block in `algo_simple.c`

`algo_simple.c` contains `#include "push_swap.h"` **twice** (lines 13 and 39) and the
same 20-line "ALGORITHM SUMMARY" comment **twice** (lines 15-37 and 41-62). Harmless
to the compiler, but an evaluator reading top-to-bottom will notice and it invites
"did you actually read your own file?".

Delete lines 39-62.

---

## Suggested order of work

1. Move files to root + `.gitignore` + drop dead files (30 min)
2. Fix `--bench` percentage, `CC = cc`, `free_and_exit`, duplicate block (20 min)
3. `make re && norminette *.c *.h && ` re-run the checker suite (10 min)
4. Write `README.md` (the long pole — budget a real session)
5. Read `05_defense_prep.md` together, out loud, before the defense
