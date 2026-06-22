# Step 15 — Bonus: the `checker` program

> **Goal:** A second program that *executes* a list of operations on stack `a` and
> says whether the result is sorted. **Only graded if the mandatory part is
> perfect** (all benchmarks passing, no errors). Don't start this until mandatory
> is done.

## What checker does (from the subject)

1. Takes stack `a` as arguments (first arg = top), same parsing rules as push_swap.
   No argument → stop, display nothing.
2. **Reads operations from stdin**, one per line (`\n`-terminated), until EOF.
3. **Executes** them on the stack (no printing of the ops).
4. After all ops: if `a` is sorted **and** `b` is empty → print `OK\n` to stdout.
   Otherwise → print `KO\n` to stdout.
5. On error → `Error\n` to **stderr**. Errors include: non-integer args,
   out-of-range, duplicates, **an instruction that doesn't exist or is malformed.**

Example:
```
$> ./checker 3 2 1 0
rra
pb
sa
rra
pa
OK
```

## How it reuses your existing work

- **Parsing & error handling:** identical to Step 05. Share the code (or copy it
  into the bonus files — remember bonus must be in `_bonus.{c,h}` files per the
  common instructions, and built by the `bonus` Makefile rule).
- **The operation mutations:** the *pure* `do_sa`/`do_pb`/... functions from Step
  06 — **without** the print. This is exactly why Step 06 told you to separate
  mutation from emission. Here you call the mutation only.
- **`is_sorted` + "b empty":** the success test.

## The new piece: reading and validating instructions from stdin

- Read line by line. The allowed tokens are exactly:
  `sa sb ss pa pb ra rb rr rra rrb rrr`.
- Any other line (typo, extra spaces, empty where not expected, an unknown
  instruction) → it's a **malformed instruction** → `Error\n` to stderr, exit.
  Be strict: `"ra "` with a trailing space or `"raa"` is invalid.
- Map each valid token to its mutation and apply it immediately (or buffer then
  apply — applying immediately is simplest).

Concepts:
- **Reading a line safely:** use `read` (allowed) into a buffer; or a
  `get_next_line`-style reader (you may bring it from libft). Watch the buffer/leak
  discipline. Handle the final line possibly lacking a trailing `\n` before EOF.
- **Token → op dispatch:** a small table or if/else chain comparing the whole line
  (after stripping the `\n`) to each valid op string. Reject anything else.

## Manual
1. Set up `bonus/checker_bonus.c` (+ helpers in `_bonus.c` files). Add the `bonus`
   rule to the Makefile (separate binary `checker`, no relink, same flags).
2. Reuse parse + pure-mutation + `is_sorted`.
3. Write the stdin reader and the token validator/dispatcher.
4. After EOF: test `is_sorted(a) && b_empty` → print `OK` / `KO`.
5. Handle every error path to stderr with no leaks.

## Pitfalls
- **Printing OK/KO to stderr** — they go to **stdout**. Only `Error` goes to
  stderr.
- **Not validating instructions** — a garbage line must produce `Error`, not a
  crash or a silent skip.
- **Forgetting `b` must be empty** for OK — a sequence that sorts `a` but leaves
  something in `b` is `KO`.
- **Leaks on the read loop** — free your line buffers.
- **Bonus norm:** bonus files are norm-checked too; a norm error there zeroes the
  bonus.

## Verify
```
./push_swap $ARG | ./checker $ARG          # OK  (your own sorter)
echo -e "sa\nrra\npb" | ./checker 3 2 1 0  # KO  (doesn't sort)
echo "rotate" | ./checker 3 2 1            # Error (bad instruction, stderr)
./checker 3 2 one 0                        # Error (bad arg, stderr)
./checker                                   # nothing
```
Compare against the provided `checker_linux` binary on many random op streams.

➡ Next: `16_testing.md`
