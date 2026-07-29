# Test Results — 2026-07-28

Environment: macOS 14.3 (arm64), `gcc` = Apple clang, norminette 3.3.58,
`checker_Mac` (school binary) used as the oracle for every correctness run.

---

## 1. Build

```
make re
```
Zero warnings under `-Wall -Wextra -Werror`. 17 objects + link.

**Relink check** — `make` twice in a row:
```
make: Nothing to be done for `all'.
make: Nothing to be done for `all'.
```
✅ No relink.

## 2. Norminette

```
norminette *.c *.h
```
All 18 files → `OK!`
(`algo_complex.c`, `algo_complex_utils.c`, `algo_medium.c`, `algo_simple.c`,
`algorithm_utils.c`, `benchmark.c`, `build_stack.c`, `disorder.c`, `flags.c`,
`ft_split.c`, `main.c`, `ops_push.c`, `ops_rev_rotate.c`, `ops_rotate.c`,
`ops_swap.c`, `parser.c`, `stack_utils.c`, `push_swap.h`)

`tests/bench_main.c` was **not** checked — it is not in `SRC` and is entirely
commented out. Delete it before submission rather than risk it.

## 3. Correctness — exhaustive

Every permutation of `1..n` for **n = 1 to 7**, run through **all five modes**
(default, `--simple`, `--medium`, `--complex`, `--adaptive`), each output piped
into `checker_Mac`:

```
total failures: 0
```

That is 13 700 permutations × 5 modes ≈ **28 000 verified sorts**, zero `KO`.

## 4. Correctness — randomised

200 runs, random size from {4, 5, 8, 13, 27, 50, 99, 101, 250}, values drawn from
`-1000..999` (negatives + non-contiguous), random strategy flag:

```
random stress failures: 0
```

## 5. Performance — 100 numbers

| Mode | Runs | Avg | Min | Max | KO |
|------|------|-----|-----|-----|----|
| default (adaptive) | 20 | **577** | 516 | 623 | 0 |
| `--simple` | 10 | 1363 | 1276 | 1525 | 0 |
| `--medium` | 20 | 574 | 547 | 603 | 0 |
| `--complex` | 20 | 572 | 529 | 623 | 0 |

Excellent band is < 700. **Default: pass.**
Note `--simple` at 1363 is still inside the < 2000 pass band and the < 1500 good
band, which is a nice thing to point out at defense.

## 6. Performance — 500 numbers

| Mode | Runs | Avg | Min | Max | KO |
|------|------|-----|-----|-----|----|
| default (adaptive) | 10 | **5115** | 4967 | 5329 | 0 |
| `--medium` | 10 | 5056 | 4948 | 5173 | 0 |
| `--complex` | 10 | 5178 | 4843 | 5384 | 0 |
| `--simple` | 3 | 31419 | 30234 | 32244 | 0 |

Excellent band is < 5500. **Default: pass.**
`--simple` at ~31 000 is expected for an O(n²) strategy on random input and is not
a violation — the benchmark targets apply to the program's default behaviour.

## 7. Performance vs. disorder (n = 500) — ⚠️ the interesting one

Inputs constructed to hit a target disorder, then run with the **default** adaptive
selector:

| Disorder | Branch taken | Ops | vs. 12000 pass threshold |
|----------|--------------|-----|--------------------------|
| 0.45 | Medium — O(n√n) | 5539 | ✅ |
| 0.30 | Medium — O(n√n) | 5566 | ✅ |
| **0.19** | **Simple — O(n²)** | **19138** | ❌ over |
| **0.16** | **Simple — O(n²)** | **17298** | ❌ over |
| **0.12** | **Simple — O(n²)** | **13965** | ❌ over |
| **0.10** | **Simple — O(n²)** | **12240** | ❌ over |
| 0.08 | Simple — O(n²) | 10170 | ✅ |
| 0.05 | Simple — O(n²) | 7102 | ✅ |
| ~0.003 | Simple — O(n²) | 1176 | ✅ |

**The window `0.09 ≲ disorder < 0.20` at n = 500 exceeds 12000 operations.**
Random input never lands there (random ≈ 0.5), so the published benchmark is safe.
But if an evaluator hands you a shuffled-then-partially-sorted 500-element list you
will print ~19 000 ops. Analysis and options in `04_code_findings.md` §3.

At n = 100 the same disorder range is harmless: d≈0.19 → 897 ops, d≈0.10 → 660 ops.

## 8. Error handling

| Input | stderr | exit | Verdict |
|-------|--------|------|---------|
| *(no args)* | — | 0 | ✅ silent, prompt back |
| `0 one 2 3` | `Error` | 1 | ✅ |
| `3 2 3` (duplicate) | `Error` | 1 | ✅ |
| `2147483648` | `Error` | 1 | ✅ INT_MAX+1 |
| `-2147483649` | `Error` | 1 | ✅ INT_MIN-1 |
| `1 99999999999` | `Error` | 1 | ✅ long overflow guarded |
| `--bogus 1 2` | `Error` | 1 | ✅ unknown flag |
| `1 2 --` | `Error` | 1 | ✅ |
| `+ 1` | `Error` | 1 | ✅ sign with no digits |
| `""` | `Error` | 1 | ✅ |
| `"" 1` | `Error` | 1 | ✅ matches the PDF's checker example |
| `"1 2 x"` | `Error` | 1 | ✅ split path validates too |
| `--bench` *(flags only, no numbers)* | — | 0 | ✅ silent |
| `42` | — | 0 | ✅ single element, nothing to do |
| `-2147483648 2147483647 0 -1` | — | 0 | ✅ `OK`, boundary values accepted |

## 9. Memory

macOS `leaks -atExit` on both success and every error path:
```
0 leaks for 0 total leaked bytes
```

⚠️ Caveat: `leaks` only reports *unreachable* blocks. Code reading shows
`free_and_exit` (`parser.c:103`) calls `free_stack(stack)` — which frees the **nodes**
— but never `free(stack)` for the `t_stack` struct itself. Under valgrind this shows
as **"still reachable"**, not "definitely lost". Cosmetic, but trivially fixable and
some evaluators do read the summary line. See `04_code_findings.md` §1.

`main.c` is clean — `clean_exit` frees nodes *and* both structs.

## 10. Scaling beyond the subject (informational)

| n | Ops | Wall time | Result |
|---|-----|-----------|--------|
| 1000 | 14121 | 0.06 s | OK |
| 2000 | 36839 | 2.96 s | OK |
| 5000 | — | > 120 s (timed out) | not completed |

Cause: `assign_cost` in the complex sorter is O(n) per node × n nodes per iteration
× n iterations ⇒ O(n³) **CPU** work (the *operation count* is still fine). The
subject never tests above 500, so this is not a defect — just don't demo n = 5000.

## Reproducing

Scripts used live in the session scratchpad; the two that matter:

```bash
# exhaustive
python3 - <<'PY'
import itertools, subprocess
fail=[]
for n in range(1,8):
    for perm in itertools.permutations(range(1,n+1)):
        args=[str(x) for x in perm]
        for flag in ([],["--simple"],["--medium"],["--complex"],["--adaptive"]):
            out=subprocess.run(["./push_swap"]+flag+args,capture_output=True,text=True)
            chk=subprocess.run(["./checker_Mac"]+args,input=out.stdout,capture_output=True,text=True)
            if chk.stdout.strip()!="OK": fail.append((n,perm,flag))
print("failures:",len(fail))
PY

# benchmark one size
ARG=$(python3 -c "import random;a=list(range(1,501));random.shuffle(a);print(' '.join(map(str,a)))")
./push_swap $ARG | wc -l
./push_swap $ARG | ./checker_Mac $ARG
```
