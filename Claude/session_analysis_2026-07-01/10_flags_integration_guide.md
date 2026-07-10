# Step 10 — Adding `--bench` and the algorithm flags to `main`

> **Goal:** make `push_swap` accept `--bench` and the strategy selectors
> (`--simple`, `--medium`, `--complex`, `--adaptive`) *without* breaking the
> number parser. This is a documentation/how-to file — no code was changed. Apply
> it yourself so you understand every line before your defense.

---

## 1. Why the current parser breaks on flags

Trace what happens today when you run `./push_swap --bench 5 4 3`:

1. `main` calls `build_stack(a, argc, argv)`.
2. `build_stack` loops over **every** `argv[i]` and calls `add_value`.
3. `add_value` runs `error_syntax(tok)` first. For `"--bench"` the first char is
   `-`, `i++`, then `-` is **not** a digit → `error_syntax` returns `1` →
   `free_and_exit` → prints `Error`.

So the very first flag kills the program. There is a **second**, quieter bug:

```c
if (argc == 2)
    parse_split(a, argv[1]);   // "treat argv[1] as one space-separated string"
else
    ... loop each argv as a number ...
```

`argc == 2` is your signal for *"the user passed one quoted list like `"5 4 3"`"*.
The moment a flag occupies an `argv` slot, that count is wrong:
`./push_swap --bench "5 4 3"` has `argc == 3`, so it would go down the
"multiple numbers" branch and try to parse `--bench` as a number.

**Conclusion:** the fix is not "add more `if`s" — it's to *classify each token
once* as flag-or-number, apply flags as state, and only feed **numbers** to the
existing number parser.

---

## 2. The design — separate "what to sort" from "how to sort"

Three independent things come out of the command line (see `12_cli_and_dispatch.md`):

| Piece      | Where it goes                          |
|------------|----------------------------------------|
| numbers    | stack `a` (already handled)            |
| strategy   | new `t_strategy strategy` field on `a` |
| bench flag | new `bool bench` field on `a`          |

Key idea: **the flags are just extra state on the context struct you already
pass everywhere.** You do *not* need to add new parameters to `sort_dispatch`,
the sorters, or the ops. `a` already travels through the whole program, so put
`bench` and `strategy` on it and read them where needed.

Why this is the "most efficient" way for *your* codebase:
- No new plumbing threaded through 10+ functions.
- Reuses your existing `add_value` / `error_syntax` / `parse_split` untouched for
  the numeric path.
- One tiny new file (`flags.c`) keeps you under the Norm's 5-functions-per-file
  limit (`build_stack.c` is already at 5).

---

## 3. Step-by-step

### Step 3.1 — Add the flag state to `push_swap.h`

Define the strategy enum **above** the struct (the struct will reference it), then
add two fields.

```c
typedef enum e_strategy
{
    ADAPTIVE,   /* value 0 → also the default when no selector is given */
    SIMPLE,
    MEDIUM,
    COMPLEX
}   t_strategy;
```

Inside `t_stack_check`, next to the op counters, add:

```c
    t_strategy      strategy;
    bool            bench;
```

Add prototypes for the new helpers and the missing sorter:

```c
/* --- FLAGS --- */
int     is_flag_token(char *arg);
int     parse_flags(t_stack_check *a, int argc, char **argv);
char    *first_number(int argc, char **argv);

/* medium_sorter was never declared — add it */
void    medium_sorter(t_stack_check *a, t_stack_check *b);
```

> Note: `medium.c` is **not** in the Makefile `SRC` either, so `medium_sorter`
> isn't even compiled yet. See Step 3.5.

### Step 3.2 — Initialise the new fields in `init_stack` (`build_stack.c`)

`init_stack` currently forgets `op_sb` and `op_ss` too — fix all of them:

```c
    stack->op_sb = 0;      /* was missing */
    stack->op_ss = 0;      /* was missing */
    stack->strategy = ADAPTIVE;
    stack->bench = false;
```

Default strategy is `ADAPTIVE` (enum value 0), matching the subject's rule
*"no selector → adaptive."*

### Step 3.3 — New file `flags.c` (the classifier)

Put all flag logic here so `build_stack.c` stays at 5 functions. This file has
exactly 5 functions — Norm-safe. `ft_strcmp` and `apply_flag` are `static`
(internal), the other three are exported via the header.

```c
#include "push_swap.h"

/* only used inside this file → static */
static int  ft_strcmp(char *a, char *b)
{
    int i;

    i = 0;
    while (a[i] && a[i] == b[i])
        i++;
    return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/* a flag is exactly a token starting with "--"; a negative number
   like -42 starts with a single '-', so it is NOT caught here */
int is_flag_token(char *arg)
{
    return (arg[0] == '-' && arg[1] == '-');
}

/* set the state a recognised flag represents; unknown "--xxx" is an error */
static void apply_flag(t_stack_check *a, char *arg)
{
    if (!ft_strcmp(arg, "--bench"))
        a->bench = true;
    else if (!ft_strcmp(arg, "--simple"))
        a->strategy = SIMPLE;
    else if (!ft_strcmp(arg, "--medium"))
        a->strategy = MEDIUM;
    else if (!ft_strcmp(arg, "--complex"))
        a->strategy = COMPLEX;
    else if (!ft_strcmp(arg, "--adaptive"))
        a->strategy = ADAPTIVE;
    else
        free_and_exit(a, NULL, false);   /* unknown flag → Error */
}

/* single pass: apply every flag, return how many NUMBER args exist */
int parse_flags(t_stack_check *a, int argc, char **argv)
{
    int i;
    int nums;

    i = 1;
    nums = 0;
    while (i < argc)
    {
        if (is_flag_token(argv[i]))
            apply_flag(a, argv[i]);
        else
            nums++;
        i++;
    }
    return (nums);
}

/* return the first non-flag arg (used for the quoted-string case) */
char *first_number(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc)
    {
        if (!is_flag_token(argv[i]))
            return (argv[i]);
        i++;
    }
    return (NULL);
}
```

**Why a count instead of the old `argc == 2` check:** the "is this one quoted
list?" question is really *"is there exactly one number argument?"* — regardless
of how many flags surround it. `parse_flags` returns that count, so the decision
becomes flag-position-independent (the subject requires `--bench` to work
anywhere in the args).

### Step 3.4 — Rewrite `build_stack` (`build_stack.c`)

Keep `add_value`, `parse_split`, `free_tokens` **exactly as they are**. Only the
top-level `build_stack` changes:

```c
void build_stack(t_stack_check *a, int argc, char **argv)
{
    int i;
    int nums;

    nums = parse_flags(a, argc, argv);   /* applies flags, counts numbers */
    if (nums == 0)
        return ;                         /* only flags → nothing to sort */
    if (nums == 1)
        parse_split(a, first_number(argc, argv));  /* quoted "5 4 3" case */
    else
    {
        i = 1;
        while (i < argc)
        {
            if (!is_flag_token(argv[i]))
                add_value(a, argv[i], NULL, false);
            i++;
        }
    }
    if (a->size == 0)
        free_and_exit(a, NULL, false);   /* n>=1 but empty (e.g. "") → Error */
}
```

Read the two exit conditions carefully — they preserve subject-correct edge cases:

- `nums == 0` → **only flags were given** (`./push_swap --bench`). Return with an
  empty stack; `main` will print nothing (or the bench block with all-zero
  counts) and exit `0`. **Not** an error.
- `nums >= 1` **but** `a->size == 0` → the user passed something that parsed to no
  values (`./push_swap ""`). That stays an **Error**, exactly like before.

This is the whole trick: distinguishing "no numbers because only flags" (exit 0)
from "no numbers because the input was garbage" (Error).

### Step 3.5 — Wire `main` as a thin conductor (`main.c`)

Two changes: **guard the sort** for the empty-stack case, and **gate the bench
print** behind `a->bench`.

```c
int main(int argc, char **argv)
{
    t_stack_check   *a;
    t_stack_check   *b;

    if (argc < 2)
        return (0);
    a = init_stack();
    b = init_stack();
    if (!a || !b)
    {
        write(2, "Error\n", 6);
        return (1);
    }
    build_stack(a, argc, argv);
    if (a->size > 0)                         /* skip work when only flags given */
    {
        a->disorder = compute_disorder(a);   /* compute BEFORE any moves */
        if (!is_sorted(a))
            sort_dispatch(a, b);
    }
    if (a->bench)                            /* bench is orthogonal to strategy */
        print_benchmark_summary(a, b);
    free_stack(a);
    free_stack(b);
    free(a);
    free(b);
    return (0);
}
```

And make `sort_dispatch` honour the forced strategy. Tiny sizes are still
special-cased first (2 elements is always `sa`, 3 is always `three_sorter` — a
forced flag can't beat those):

```c
static void sort_dispatch(t_stack_check *a, t_stack_check *b)
{
    if (a->size == 2)
        sa(a);
    else if (a->size == 3)
        three_sorter(a);
    else if (a->strategy == SIMPLE)
        simple_sorter(a, b);
    else if (a->strategy == MEDIUM)
        medium_sorter(a, b);
    else
        complex_sorter(a, b);   /* COMPLEX and ADAPTIVE both route here for now */
}
```

> `ADAPTIVE` currently falls through to `complex_sorter`, which is exactly your
> old default (`size > 3 → complex_sorter`). Later, adaptive can branch on
> `a->disorder` to pick simple/medium/complex — but that's Step 11, not this task.

### Step 3.6 — Makefile

Add the new file **and** `medium.c` (which was missing) to `SRC`:

```make
SRC = algorithm_utils.c build_stack.c disorder.c flags.c ft_split.c main.c \
      medium.c ops_push.c ops_rev_rotate.c ops_rotate.c ops_swap.c parser.c \
      simple.c sort_three.c stack_utils.c complex.c complex_utils.c
```

---

## 4. Order of edits (so it always compiles)

1. `push_swap.h` — enum, struct fields, prototypes.
2. `build_stack.c` — `init_stack` fields + new `build_stack`.
3. `flags.c` — new file.
4. `main.c` — dispatch + bench gate.
5. `Makefile` — add `flags.c` and `medium.c`.
6. `make re`.

---

## 5. How to verify

```sh
# flag anywhere, still sorts correctly
./push_swap --complex 5 4 3 2 1 | ./checker 5 4 3 2 1     # OK
./push_swap 5 4 3 2 1 --bench | ./checker 5 4 3 2 1       # OK

# quoted list + flag
./push_swap --bench "5 4 3 2 1"                           # sorts + prints bench

# no numbers, only a flag → nothing / exit 0, NOT Error
./push_swap --simple ; echo "exit=$?"                    # exit=0

# unknown flag → Error
./push_swap --turbo 3 2 1                                 # Error

# garbage still errors
./push_swap "" ; echo "exit=$?"                          # Error, exit=1
./push_swap --adaptive 0 one 2                            # Error

# op count must match with and without bench
./push_swap $ARG | wc -l                                 # == total_ops in bench
```

---

## 6. Two follow-ups (out of scope here, but flag them for the team)

1. **`print_benchmark_summary` prints to stdout with `printf`.** Both are
   problems: (a) `printf` from libc is **not allowed** at 42 — use your own
   `ft_printf`/`write`; (b) the bench block **must** go to **stderr (fd 2)**, or
   it pollutes the `push_swap | checker` pipe and the checker reports `KO`.
   See `13_benchmark_mode.md`. This is separate from wiring the flag — the flag
   wiring above is correct regardless.
2. **`--bench` output format** should eventually match the subject
   (disorder %, strategy label, per-op counts). Your current summary is close but
   isn't the subject's format yet.

Neither blocks the flag integration — they're the next clean-up passes.
