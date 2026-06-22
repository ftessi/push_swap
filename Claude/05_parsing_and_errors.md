# Step 05 — Parsing & Error Handling

> **Goal:** Turn `argv` into a validated stack `a`, or print `Error\n` to **stderr**
> and exit cleanly. Do this *before* any sorting. A sort that runs on bad input is
> a bug factory.

## Exactly what counts as an error (from the subject)

Print `Error` + `\n` to **standard error** (not stdout) and exit, when:
- An argument is **not an integer** (e.g. `one`, `3a`, empty string `""`, a lone
  `+`/`-`, `--` not a recognized flag).
- An integer is **out of `int` range** (bigger than `INT_MAX` / smaller than
  `INT_MIN`).
- There are **duplicate** values.

Also handle:
- **No arguments** → display nothing, exit success (give the prompt back). This is
  NOT an error.
- Arguments passed as **one quoted string** `"4 67 3 87 23"` vs separate args
  `4 67 3 87 23` — the usage examples show both. Decide whether you split on
  spaces inside an argument. (The subject's examples use `$ARG` which the shell
  splits, so you may not strictly need in-arg splitting, but handling it is more
  robust and is commonly tested.)

## Concepts you need

### Robust string→int conversion
`atoi` is **not enough** — it silently overflows and silently stops at junk.
You must:
1. Skip optional leading sign.
2. Require at least one digit.
3. Reject any non-digit character.
4. Detect overflow **as you build the number**, before it wraps. The standard
   technique: accumulate and check against `INT_MAX`/`INT_MIN` *before* each
   multiply-add, or accumulate as a wider type (`long`) and compare to int bounds
   at the end. Be careful: `INT_MIN` has larger magnitude than `INT_MAX`, so
   `-2147483648` is valid but `2147483648` is not.

Write your own `ft_atoi_strict` (or `safe_atol`) that signals failure. Don't reuse
plain libft `ft_atoi`.

### Duplicate detection
After converting all values into an array/stack, check no two are equal. Simplest:
nested compare (O(n²)) — totally fine for n ≤ 500. If you want O(n log n), sort a
*copy* and scan neighbors. Don't sort the real stack to check — you need its
original order for the disorder metric and for actually sorting.

### Flag vs number disambiguation
The strategy selectors (`--simple` etc.) and `--bench` can appear among the
arguments. Your parser must:
- Recognize known flags and record them (not treat them as numbers).
- Reject unknown `--something` as an error (or as a non-integer — your choice, but
  be consistent and document it).
- Everything else must parse as a valid int.

Keep flag parsing logic close to, but conceptually separate from, number parsing —
Step 12 covers dispatch in detail. Here, just make sure a flag isn't mistaken for
a number and vice versa.

## Manual: build the parser

1. Decide your argument model. Two common shapes:
   - **Strict:** numbers are separate `argv` entries; flags too.
   - **Lenient:** also accept a single quoted string of space-separated numbers
     (split it yourself).
   Pick one, write it in a comment, and be consistent.
2. First pass: walk `argv`, classify each token as flag / number / invalid. Any
   invalid → error path.
3. Convert numbers with your strict converter. Any failure → error path.
4. Build stack `a` in order (remember: first number = top).
5. Check duplicates. Any → error path.
6. On any error: free whatever you allocated (no leaks even on the error path!),
   write `Error\n` to fd 2, and `exit`. Centralize this in one
   `error_exit(ctx)` helper so cleanup is consistent.

## Pitfalls (these fail real evaluations)

- **Leaking on the error path.** Evaluators run valgrind on bad input too. Your
  `Error` exit must free everything already allocated.
- **Writing Error to stdout.** It must go to **stderr** (fd 2).
- **Treating no-args as an error.** No args = print nothing, exit 0.
- **`atoi` overflow.** `./push_swap 2147483648` must say `Error`.
- **Empty string / sign-only.** `""`, `"+"`, `"-"` are errors.
- **Already-sorted or single element.** Not errors — they just need (possibly
  zero) operations. `./push_swap 42` prints nothing and exits 0.

## Verify
Build a small script of cases and eyeball each:
```
./push_swap 0 one 2 3        # Error
./push_swap 3 2 3            # Error (dup)
./push_swap 2147483648       # Error (overflow)
./push_swap ""               # Error
./push_swap                  # (nothing, exit 0)
./push_swap 42               # (nothing, exit 0)
./push_swap 2 1 3 6 5 8      # (operations)
```
Run the error cases under valgrind — **zero leaks** required.

➡ Next: `06_operations.md`
