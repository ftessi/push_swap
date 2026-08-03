# Evaluation Sheet Dry-Run (2026-08-03)

Every graded item from `Push Swap Evaluation.pdf` (8 pages), run against the current
`Code/` using the provided `checker_Mac`. One item needs a decision from you — it is
at the bottom and it is a single word.

---

## Result: every mandatory section passes, most in the top band

| Section | Requirement | Result |
|---------|-------------|--------|
| Norminette | no errors, else evaluation stops | ✅ 20/20 files `OK!` |
| Compilation | `NAME/all/clean/fclean/re`, `-Wall -Wextra -Werror`, no relink | ✅ all present, no relink |
| Memory leaks | major leaks ⇒ grade 0 | ✅ 0 bytes on every path incl. errors |
| Error management | ≥3 of 4 | ✅ **4 / 4** |
| Strategy selection | ≥3 of 5 | ✅ **5 / 5** |
| Identity (sorted input) | ≥3 of 4 | ✅ **4 / 4** |
| 3 numbers | ≤5 ok, ≤3 good | ✅ **1-2 ops** |
| 5 numbers | ≤15 ok, ≤12 good | ✅ **6-10 ops** |
| Benchmark mode | disorder %, strategy+class, total, per-op counts | ✅ all four present |
| 100 numbers | <2000 pass, <1500 good, **<700 excellent** | ✅ **572-606** |
| Strategy comparison | complex should beat simple | ✅ 216 vs 387, 251 vs 372 |
| 500 numbers | <12000 pass, <8000 good, **<5500 excellent** | ✅ **5025-5334** |
| Live coding `--count-only` | ≤10 min | ✅ prepared — see `07_live_coding_count_only.md` |
| Bonus checker | only if mandatory is perfect | ➖ not implemented |

### The detail

**Error management — 4/4**
```
./push_swap 1 two 3          -> Error   (stderr)
./push_swap 1 2 2            -> Error   (stderr)
./push_swap 1 2 2147483648   -> Error   (stderr)
./push_swap                  -> (nothing, prompt back)
```

**Strategy flags on `5 4 3 2 1` — 5/5**, all `OK` against the checker:
`--simple` 8 · `--medium` 10 · `--complex` 8 · `--adaptive` 8 · no flag 8
(no flag matches `--adaptive` exactly, which is the specific thing they check)

**Identity — 4/4.** `42`, `2 3`, `0 1 2 3`, `0 1 2 3 4 5 6 7 8 9` all print nothing.

**3 numbers:** `2 1 0`→2, `0 2 1`→2, `1 0 2`→1, `2 0 1`→1, `1 2 0`→1. All `OK`, all in
the "good" band (≤3).

**5 numbers:** `1 5 2 4 3`→9, `5 1 4 2 3`→6, `3 5 1 4 2`→9, `4 2 5 1 3`→10. All `OK`,
all in the "good" band (≤12).

**Benchmark mode.** Their exact command,
`./push_swap --bench --simple 5 4 3 2 1 2>bench.txt >/dev/null && cat bench.txt`:

```
 ================== OPS SUMMARY ==================
SWAPS    | SA : 1	| SB : 0	| SS : 0
PUSHES   | PA : 2	| PB : 2
ROTATES  | RA : 3	| RB : 0	| RR : 0
REV-ROT  | RRA: 0	| RRB: 0	| RRR: 0
--------------------------------------------------
METRICS  | Initial Disorder: 100.00%
         | SELECTED : Simple / O(n^2)
         | EXECUTED : Simple / O(n^2)
--------------------------------------------------
 TOTAL MOVES: 8
==================================================
```

Their two disorder spot-checks pass exactly:
`1 2 3 4 5` → **0.00%** · `5 4 3 2 1` → **100.00%**

> Worth noting: this section is *only* passable because of the percentage fix from
> 2026-07-29. The old code printed `1.00` for reverse-sorted input, and the sheet asks
> for "close to 100.00%". That one-line change is the difference between a tick and a
> cross here.

**100 numbers** (`shuf -i 1-500 -n 100`, 3 runs): 606, 572, 602 — all `OK`, **excellent** band.

**500 numbers** (`shuf -i 1-1000 -n 500`, 3 runs): 5025, 5291, 5334 — all `OK`, **excellent** band.

**50 numbers, strategy comparison** (`shuf -i 1-200 -n 50`): complex uses roughly half
the operations of simple, which is the trend they want to see.

---

## ⚠️ One decision for you: `activity` vs `project` in the README

The evaluation sheet and the subject PDF **disagree on the required first line**.

| Source | Required wording |
|--------|------------------|
| Subject PDF, Ch. VII | *"This **project** has been created as part of the 42 curriculum by …"* |
| **Evaluation sheet, README compliance check** | *"This **activity** has been created as part of the 42 curriculum by \<login1\>, \<login2\>"* |

Your README currently says **"project"** — it follows the subject.

The evaluator ticks a Yes/No box while reading the *evaluation sheet*, so the sheet's
wording is what they will most likely compare against. But the subject is the
specification, and an evaluator who checked it would find "project" correct.

**My recommendation: change it to "activity."** The evaluator has the sheet open in
front of them; a word-for-word match on a Yes/No compliance box is worth more than
being right against a document they may not reopen. And if they *do* quote the subject,
"activity" is trivially defensible — you point at their own scale and they move on.

It is one word in `Code/README.md` line 1. I have not changed it — your call:

```
*This activity has been created as part of the 42 curriculum by ftessi, umutkilicaslan.*
```

Everything else in that compliance check already passes: exactly 2 logins, italicised
first line, Description, Instructions, Resources with the AI usage explanation, the
per-algorithm justification covering all four strategies, and the contributions section.

---

## Reminders before the defense

1. **Both of you must be present**, and both must be able to explain any part.
   "No report: 0" and "Can't support / explain code" are flags on this sheet.
2. **Verify the Contributions table** in the README is accurate — the sheet checks that
   it "clearly documents each learner's contributions", and it is the one section that
   was inferred from git rather than known.
3. **`git clone` into an empty folder** is part of their procedure — make sure what you
   push actually builds from scratch. Push the *contents* of `Code/`, and **do not
   upload `checker_Mac`** (it is their binary, and shipping it looks bad).
4. **No bonus.** The checker program is not implemented, and the sheet is explicit that
   bonuses are ignored unless the mandatory part is perfect anyway. Don't present the
   provided `checker_Mac` as your own work — that is the kind of thing that gets read
   as cheating.
5. The one question worth pre-empting is the low-disorder trade-off — see
   `05_defense_prep.md` §1 and the README's "Known trade-off, stated honestly".
