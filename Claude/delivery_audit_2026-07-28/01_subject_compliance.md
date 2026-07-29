# Subject Compliance — clause by clause

Source: `Push Swap.pdf` v1.1, 22 pages. Each row quotes/paraphrases the subject
requirement and states what the code actually does.

---

## Chapter II — Common Instructions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| II.1 | Written in C | ✅ | 17 `.c` + 1 `.h` |
| II.2 | Norm compliant (**norm error = 0 on the project**) | ✅ | `norminette *.c *.h` → 18× `OK!` |
| II.3 | No segfault / bus error / double free | ✅ | No crash in ~30 000 runs, incl. exhaustive n=1..7 |
| II.4 | All heap memory freed | ⚠️ | See `04_code_findings.md` §1 — `t_stack` struct leaks on error paths |
| II.5 | Makefile with `-Wall -Wextra -Werror`, **using `cc`** | ⚠️ | Flags correct, but `CC = gcc` (Makefile:15). Subject says `cc` |
| II.6 | Makefile must not relink | ✅ | Second `make` prints "Nothing to be done for `all'." |
| II.7 | Rules `$(NAME) all clean fclean re` | ✅ | Makefile:32-53, plus `.PHONY` |
| II.8 | `bonus` rule if bonuses submitted | ➖ | No bonus submitted, so no rule needed |
| II.9 | Only work in the Git repo is graded | ❌ | See §Delivery below |

## Chapter VI.1 — Group project requirements

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| VI.1.a | Exactly 2 learners | ✅ | `git shortlog`: `umutkilicaslan` (42 commits), `ftessi` (19) |
| VI.1.b | Both contribute meaningfully | ✅ | Both appear across parsing, ops, and algorithms |
| VI.1.c | **README.md must indicate both learners' contributions** | ❌ | **No README.md exists** |
| VI.1.d | Both present at defense, both can explain any part | 🔵 | Your job — `05_defense_prep.md` |

## Chapter VI.2 — The rules

| Requirement | Status | Evidence |
|-------------|--------|----------|
| 2 stacks a and b, b starts empty | ✅ | `main.c:87,93` — B allocated only after A validates |
| Sort ascending in a | ✅ | `checker_Mac` returns `OK` on every test |
| 11 operations implemented with exact semantics | ✅ | `ops_*.c`; `sa/sb/ss` no-op below 2 elements, `pa/pb` no-op on empty source |

Semantics spot-checked against the PDF's worked example (`2 1 3 6 5 8`): our output
is 7 ops, the subject's illustration is 12 — we are strictly better, and the
checker confirms it.

## Chapter VI.3 — Algorithm requirements

| # | Requirement | Status | Implementation |
|---|-------------|--------|----------------|
| VI.3.1 | Complexity claimed = cost in **generated push_swap operations** | ✅ | Claims are stated in ops, not CPU time. See `05_defense_prep.md` §2 |
| VI.3.2 | Disorder = inversions / total pairs, in [0,1] | ✅ | `disorder.c:15-40` is a literal transcription of the PDF pseudocode |
| VI.3.2 | **Measured before any moves** | ✅ | `main.c:99` — computed after `build_stack`, before `sort_dispatch` |
| VI.3.3.1 | Simple O(n²) | ✅ | `algo_simple.c` — selection/insertion hybrid via `target_the_b` / `target_the_a` |
| VI.3.3.2 | Medium O(n√n) | ⚠️ | `algo_medium.c` — chunked, but chunk size is a **constant** (12 or 30), not `√n`. See `04_code_findings.md` §4 |
| VI.3.3.3 | Complex O(n log n) | ✅ | `algo_complex.c` — Turk / cost-based greedy |
| VI.3.3.4 | Adaptive: `<0.2`→O(n²), `[0.2,0.5)`→O(n√n), `≥0.5`→O(n log n) | ✅ | `main.c:55-60` implements exactly these thresholds |
| VI.3.3.4 | **Document thresholds + techniques + complexity argument in README** | ❌ | No README |

## Chapter VI.5 — The push_swap program

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Program named `push_swap` | ✅ | Makefile:13 |
| Global variables forbidden | ✅ | Verified — all state in `t_stack`, passed by pointer |
| First argument = top of stack | ✅ | `node_connecter` appends to tail; argv order preserved |
| `--simple / --medium / --complex / --adaptive` | ✅ | `flags.c:55-70` |
| `--adaptive` is the default | ✅ | `init_stack` sets `strategy = ADAPTIVE` (`build_stack.c:63`) |
| Operations separated by `\n` and nothing else | ✅ | Each op is a single `write(1, "xx\n", …)` |
| Selector must work for **all** valid inputs, any size/disorder | ✅ | Exhaustive n=1..7 × all 5 modes → 0 failures |
| No parameters → print nothing, return prompt | ✅ | `main.c:85-86`, exit 0, silent |
| Error → `"Error\n"` on **stderr** | ✅ | `parser.c:119` writes to fd 2 |
| Errors: non-integer, out of int range, duplicates | ✅ | `error_syntax`, `ft_atoi_ps` bounds, `error_duplicate` |
| Binary embeds all four strategies | ✅ | Single binary, all linked |
| **Strategy name + complexity class available in `--bench`** | ✅ | `benchmark.c:78-97` |
| `--bench` shows **disorder as % with two decimals** | ❌ | Prints `0.13`, not `13.00%` — see `03_blockers.md` §3 |
| `--bench` shows strategy name + complexity class | ✅ | e.g. `Adaptive (Medium) / O(n√n)` |
| `--bench` shows total number of operations | ✅ | `TOTAL MOVES: 5115` |
| `--bench` shows count of each of the 11 op types | ✅ | All 11 printed |
| **`--bench` output on stderr, only when flag present** | ✅ | Verified: `2>/dev/null` leaves a clean op stream |

## Chapter VI.6 — Performance Benchmark

| Target | Threshold | Measured | Band |
|--------|-----------|----------|------|
| 100 random, pass | < 2000 | **577 avg** (516–623, 20 runs) | ✅ excellent |
| 100 random, good | < 1500 | 577 | ✅ |
| 100 random, excellent | < 700 | 577 | ✅ |
| 500 random, pass | < 12000 | **5115 avg** (4967–5329, 10 runs) | ✅ excellent |
| 500 random, good | < 8000 | 5115 | ✅ |
| 500 random, excellent | < 5500 | 5115 | ✅ |

⚠️ **Caveat:** these hold for *random* input (disorder ≈ 0.5 → complex path). On a
**partially sorted 500-element** input the adaptive rule forces the O(n²) branch and
the op count exceeds 12000. See `04_code_findings.md` §3 — this is a subject-mandated
behaviour, but you must be ready to explain it.

## Chapter VII — Readme Requirements

| Requirement | Status |
|-------------|--------|
| `README.md` at the **root** of the Git repository | ❌ **Missing entirely** |
| First line italicised: `*This project has been created as part of the 42 curriculum by <login1>, <login2>.*` | ❌ |
| "Description" section | ❌ |
| "Instructions" section (compile / run) | ❌ |
| "Resources" section + **how AI was used, for which tasks and parts** | ❌ |
| Detailed explanation & justification of the algorithms chosen | ❌ |
| Adaptive thresholds rationale + complexity argument (from VI.3.3.4) | ❌ |
| Both learners' contributions documented (from VI.1) | ❌ |

**This whole chapter is unaddressed. It is the single biggest gap.**

## Chapter VIII — Bonus

Not implemented. `checker_Mac` in `Code/` is the **school-provided binary**, not your
own program. That is fine — bonus is optional and is only assessed once the mandatory
part is perfect. Do not present `checker_Mac` as your bonus.

## Chapter IX — Submission

| Requirement | Status | Note |
|-------------|--------|------|
| Both learners listed as repo contributors | ✅ | Git history shows both |
| README documents both contributions | ❌ | No README |
| Files to submit: `Makefile`, `*.h`, `*.c` | ⚠️ | They exist, but live in `Code/` — see below |
| Be ready for a small live modification | 🔵 | `05_defense_prep.md` §4 |

### Delivery layout problem

```
repo/
├── Push Swap.pdf     (untracked — fine)
├── psodo.txt         (tracked — scratch pseudocode, remove)
├── Claude/           (untracked — fine, stays local)
└── Code/
    ├── Makefile      ← evaluator has to find this
    ├── *.c *.h
    ├── .DS_Store     ← tracked, remove
    ├── checker_Mac   ← tracked binary, remove
    └── tests/bench_main.c  ← 100% commented-out dead file
```

An evaluator clones the repo, types `make` at the root, and gets nothing. There is
also no `README.md` for them to read. Fix in `03_blockers.md` §1.
