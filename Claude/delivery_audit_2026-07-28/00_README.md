# Push_swap — Delivery Audit (2026-07-28)

> **STATUS: resolved 2026-07-29.** Every blocker and finding below has been fixed or
> explicitly decided against. Read **`06_changes_applied.md`** for what changed and why.
> The scorecard in this file is the *pre-fix* state, kept as the record of what was
> found. Two sections were corrected after implementation: `04_code_findings.md` §3
> (the suggested fix does not work) and `05_defense_prep.md` §1/§6 (answers improved).

> **Purpose:** a full pre-delivery control of the project against the *actual subject
> PDF* (`Push Swap.pdf`, v1.1, 22 pages). Everything in `Code/` was read, compiled,
> normed, leak-checked, stress-tested against `checker_Mac`, and benchmarked.
> This folder tells you **what passes, what blocks delivery, and what to fix first**.

## Verdict in one line

**The algorithms are done and correct. The delivery is not.**
Zero sorting failures in ~30 000 checked runs, benchmarks in the *excellent*
band, norminette clean — but there is **no README.md**, the sources are **not at
the repository root**, and the `--bench` disorder is **not printed as a percentage**
as the subject requires. Those are graded items, and today they would cost points.

## How to read this folder

| Doc | Covers |
|-----|--------|
| `01_subject_compliance.md` | Clause-by-clause checklist against the PDF |
| `02_test_results.md` | Every command run, with the raw numbers |
| `03_blockers.md` | Must-fix before you submit, in priority order |
| `04_code_findings.md` | Code-level findings (correctness, cleanliness, complexity claims) |
| `05_defense_prep.md` | The questions an evaluator will ask, and the honest answers |

## Scorecard

| Area | Status | Note |
|------|--------|------|
| Compiles `-Wall -Wextra -Werror` | ✅ | Clean, zero warnings |
| Makefile rules (`NAME/all/clean/fclean/re`) | ✅ | All present |
| No relink | ✅ | Second `make` → "Nothing to be done" |
| Norminette | ✅ | 18/18 files `OK!` |
| No global variables | ✅ | State lives in `t_stack` |
| Sorting correctness | ✅ | 0 failures, n=1..7 exhaustive × 5 modes + 200 random runs |
| Error handling | ✅ | Non-int, duplicate, overflow, bad flag, empty string |
| Memory (error paths) | ⚠️ | Nodes freed; the `t_stack` struct itself is not |
| 4 strategies + selector flags | ✅ | `--simple --medium --complex --adaptive` |
| Disorder metric | ✅ | Matches the PDF pseudocode exactly, measured pre-sort |
| Adaptive thresholds | ✅ | `<0.20` → simple, `<0.50` → medium, else complex |
| 100 numbers < 700 ops | ✅ | avg **577** (excellent band) |
| 500 numbers < 5500 ops | ✅ | avg **5115** (excellent band) |
| `--bench` content | ⚠️ | Disorder printed as `0.13`, subject asks for **% with two decimals** |
| **README.md** | ❌ | **Does not exist. Chapter VII is mandatory.** |
| **Repo layout** | ❌ | Sources in `Code/`, evaluator expects `make` to work at root |
| Bonus `checker` | ➖ | Not implemented, no `bonus` rule — optional, mandatory must be perfect first |

## What changed since `defense_2026-07-14`

The medium algorithm was rewritten (rank-based chunking), `flags.c` was split out,
`benchmark.c` gained the summary block, and norminette was brought to clean. Those
all check out. The gaps that remain are **packaging**, not code.
