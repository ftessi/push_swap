# Bugs & issues found — ranked

Ranked by how badly they'll bite. I describe the fix but (per this folder's rule)
leave the exact C for you and your teammate to write.

---

## 🔴 1. `ft_atoi_ps` returns wrong numbers (parser.c)

The single most important correctness bug. Three defects in one function:

```c
if (str[i] == '-' || str[i] == '+') {
    if (str[i] == '-') sign = -1;
}                       // <-- (a) never does i++, so the sign char stays in str[i]
while (str[i]) {
    res = (res * 10) + (str[i] - '0');
    res = res * sign;   // <-- (b) applied EVERY iteration, not once at the end
    if (res > 2147483647 || res < -2147483648)  // <-- (c) res is int; this is never true
        free_and_exit(stack, argv, is_split);
    i++;
}
return (res);
```

**Trace of `"-42"`:** with `sign=-1` and the `-` never skipped:
`'-'-'0' = -3 → *-1 = 3`; then `3*10+4=34 → *-1 = -34`; then `-34*10+2=-338 → *-1 = 338`.
It returns **338**. Any negative number and many positives come out wrong.

**Fix, conceptually:**
- After reading the sign, **advance the index** past the `+`/`-`.
- Accumulate **magnitude only** in the loop; apply the sign **once** at the return.
- Detect overflow with a **wider type** (`long`) or by checking *before* the multiply
  (`if (res > INT_MAX/10 ...)`), because an `int` can't hold a value larger than
  `INT_MAX` to test against. Compare the running value against `2147483647` /
  `-2147483648` as a **long**, then error out.
- Also: `error_syntax` should have already rejected non-digits, so validate first,
  then convert — don't rely on the converter to catch junk.

---

## 🔴 2. `main` is a test harness, not a real program (main.c)

`main(void)` never reads `argc/argv`, so the binary ignores its input entirely.
Nothing you build downstream matters until this is fixed.

**Fix:** make it `int main(int argc, char **argv)` that:
1. returns cleanly (exit 0, print nothing) when `argc < 2`;
2. calls the parser to validate every token and build stack A (error path →
   `"Error\n"` to stderr, free, exit 1);
3. if already sorted → print nothing, exit 0;
4. otherwise dispatch to a sort (for now: `three_sorter` when `size == 3`; later the
   strategy selector).

Keep the old harness as a separate manual test — don't delete the value, just get it
out of `main`.

---

## 🟠 3. Parser is never called; `error_syntax`/`error_duplicate` unused

The validation functions are correct but **dead code** right now — the compiler
would warn about unused functions if they weren't extern-visible via the header.
They need to be called from the parse flow (§2). Order per token:
**syntax-check → convert (overflow-check) → duplicate-check → append node.**

---

## 🟠 4. Norminette fails in every file

Confirmed with `norminette` on this machine. The recurring violations:
- **spaces used for indentation instead of tabs** (`SPACE_REPLACE_TAB`,
  `TOO_FEW_TAB`) — the biggest one, in `parser.c`, `stack_utils.c`, `sort_three.c`,
  `ops_rev_rotate.c`;
- **comments inside function bodies** (`WRONG_SCOPE_COMMENT`) — all the `//`
  explanation comments in the ops files must go (move them to these analysis docs);
- **ternaries** (`TERNARY_FBIDDEN`) and **`printf`** in `main.c`'s debug helper;
- **multiple statements per line / lines too long** (`main.c` lines 17–22, 50–51);
- **missing/!invalid 42 header** in `main.c` (line 1);
- **space before newline / double spaces** in `push_swap.h` and elsewhere;
- **function > 25 lines** (`print_stack_debug`).

**Fix:** run `norminette` after every file and clear it now, before more code piles
up. Set your editor to insert **real tabs**. This is graded and blocks the defense.

---

## 🟡 5. Missing NULL checks on `malloc` returns

- `main.c` builds nodes with `node_creator` and never checks for `NULL` before
  `node_connecter` (out-of-memory → crash). `node_connecter` guards `!new_node`, so
  it won't segfault, but a silently-dropped node is still wrong.
- The parse flow must treat a failed `node_creator` as an error (free + exit), not
  ignore it.

## 🟡 6. `stack_pusher` dereferences `src` without checking `src` itself

`if (!src->head)` assumes `src != NULL`. Fine given current callers, but add the
`!src`/`!dest` guard to match the defensive style of the other ops.

## 🟡 7. Misleading comment in `is_sorted`

The code enforces **ascending** order; the comment says "descending". Fix the comment
so nobody "corrects" the code in the wrong direction later. (`three_sorter` also
assumes ascending — they agree; only the comment is wrong.)

## 🟢 8. `free_stack` doesn't null `min`/`max`

Harmless today (they're recomputed by `stack_o_meter`), but for hygiene reset them to
`NULL` alongside `head`/`tail`.

## 🟢 9. Untracked file & a stray deletion in git

`git status` shows `Claude/05b_parsing_readiness_review.md` untracked and
`split_psodo.txt` deleted but unstaged. Decide with your teammate whether to commit
the readiness review (yes) and the pseudo-code deletion, so the tree is clean.

---

## Quick verification checklist once you've fixed 1–4

```
make re                              # builds clean, no warnings
norminette *.c *.h                   # all OK
./push_swap                          # prints nothing, exit 0
./push_swap 42                       # prints nothing, exit 0
./push_swap 3 1 2                    # prints a short valid op list (e.g. ra)
./push_swap 1 a 2 2>/dev/null        # prints NOTHING on stdout
./push_swap 1 2 2                    # Error (duplicate) on stderr
./push_swap 2147483648               # Error (overflow) on stderr
valgrind ./push_swap 1 2 two         # 0 leaks on the error path
```
A tiny checker script for the sorted output comes later (see `16_testing.md`).
