# Step 11 — Flags as *options*, not stack fields (refines Step 10)

> **Context:** Step 10 proposed storing `strategy` and `bench` **on the stack**
> (`t_stack_check`). That works, but it's the wrong home for them — as you spotted,
> `b` ends up carrying an algorithm/bench field it never reads, and it couples
> "the data" with "the policy for sorting the data." This file shows the cleaner
> design: **parse options first into their own struct, then build the stack from
> validated values.** No code was changed — this is the how-to.

---

## 0. Is this actually "more efficient"? (honest answer)

Three claims worth separating, because you'll have to defend them:

| Claim | Verdict |
|-------|---------|
| Options don't belong on the stack | ✅ **Correct.** `strategy`/`bench` are program policy, not stack data. Move them. |
| Parse-first-then-build is cleaner | ✅ **Correct.** Options parsing becomes independent of the stack; phases are clear. |
| It's "much more efficient" (runtime) | ⚠️ **No.** An intermediate value array is an *extra* pass + an *extra* `malloc`. Same O(n). The win is modularity, not speed. |
| It helps the bonus (checker) | ✅ **Partly.** The checker reuses the **number parser + stack builder** to build stack `a` from `argv`. It does **not** reuse anything for reading operations from stdin — that's new code regardless. Keeping the number parser free of push_swap-only concerns (flags, op counters) is the actual benefit. |

So: do it for **clean separation and reuse**, and say exactly that at eval. Don't
claim a speed win that isn't there.

### A note on the op counters (leave them for now)

`bench`/`strategy` are *global* → move them off the stack.
Op counters (`op_sa`, `op_pb`, …) are *per-stack* → each stack tallies its own
moves, so living on the stack is defensible. Your teammate is already reworking
"operations inside the stack," so **don't touch counters here** — just note that
the end-state could hoist a single counter set into the program context too
(see §7). Keep this change scoped to the two policy fields.

---

## 1. Target shape

Two structs, clear roles:

```c
/* pure program POLICY — parsed from flags, knows nothing about stacks */
typedef enum e_strategy { ADAPTIVE, SIMPLE, MEDIUM, COMPLEX } t_strategy;

typedef struct s_opts
{
    t_strategy  strategy;
    bool        bench;
}   t_opts;
```

`t_stack_check` stays exactly as it is today — **you add nothing to it.** (Note:
the original struct never had `strategy`/`bench`; Step 10 would have added them.
Here we simply don't.)

Data flow in `main`:

```
argv ──► read_opts()      ──► t_opts     (flags: strategy + bench)
     └─► parse_values()   ──► int values[]  (validated numbers)
                                  │
                          build_from_values() ──► stack a
                                  │
     dispatch(a, b, &opts) ◄──────┘   (opts decides which sorter)
     if opts.bench: print_bench(a, b) (reads per-stack op counters)
```

`b` is never given policy. `opts` never touches a stack. Each phase is testable
alone.

---

## 2. Phase 1 — parse the flags (`flags.c`)

Fully independent of the stack: it errors with `write`+`exit`, not `free_and_exit`.
Returns how many **number** arguments exist so Phase 2 knows the input shape.

```c
#include "push_swap.h"

static int  ft_strcmp(char *a, char *b)
{
    int i;

    i = 0;
    while (a[i] && a[i] == b[i])
        i++;
    return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/* a flag is a token starting with "--"; "-5" (single dash) is a number */
int is_flag(char *arg)
{
    return (arg[0] == '-' && arg[1] == '-');
}

static void set_opt(t_opts *opts, char *arg)
{
    if (!ft_strcmp(arg, "--bench"))
        opts->bench = true;
    else if (!ft_strcmp(arg, "--simple"))
        opts->strategy = SIMPLE;
    else if (!ft_strcmp(arg, "--medium"))
        opts->strategy = MEDIUM;
    else if (!ft_strcmp(arg, "--complex"))
        opts->strategy = COMPLEX;
    else if (!ft_strcmp(arg, "--adaptive"))
        opts->strategy = ADAPTIVE;
    else
    {
        write(2, "Error\n", 6);
        exit(1);
    }
}

/* fills opts, returns count of NON-flag (number) args */
int read_opts(t_opts *opts, int argc, char **argv)
{
    int i;
    int nums;

    opts->strategy = ADAPTIVE;   /* default when no selector given */
    opts->bench = false;
    i = 1;
    nums = 0;
    while (i < argc)
    {
        if (is_flag(argv[i]))
            set_opt(opts, argv[i]);
        else
            nums++;
        i++;
    }
    return (nums);
}
```

`ft_strcmp`, `is_flag`, `set_opt`, `read_opts` = 4 functions — room for
`first_number` (below) → still Norm-safe (≤5/file).

---

## 3. Phase 2 — parse the numbers into a validated array

This is the "send the values to the builders" part you asked for. Put it in a new
`values.c` so the number logic is a **self-contained, checker-reusable unit** with
no flag/bench code in it.

The one real complication (same as today): the numbers can arrive as either
`5 4 3` (many argv) **or** `"5 4 3"` (one quoted argv). We normalise both into one
owned `char **words`, then validate word-by-word into an `int` array.

```c
#include "push_swap.h"

/* first non-flag argv token — used for the single quoted-string case */
char    *first_number(int argc, char **argv)   /* can also live in flags.c */
{
    int i;

    i = 1;
    while (i < argc)
    {
        if (!is_flag(argv[i]))
            return (argv[i]);
        i++;
    }
    return (NULL);
}

/* copy the non-flag argv tokens into an owned NULL-terminated array
   so ownership is uniform with the ft_split case (one free path) */
static char **dup_tokens(int argc, char **argv, int nums)
{
    char    **words;
    int     i;
    int     w;

    words = malloc(sizeof(char *) * (nums + 1));
    if (!words)
        return (NULL);
    i = 1;
    w = 0;
    while (i < argc)
    {
        if (!is_flag(argv[i]))
            words[w++] = ft_strdup(argv[i]);   /* libft, or a 6-line local copy */
        i++;
    }
    words[w] = NULL;
    return (words);
}

static char **collect_words(int argc, char **argv, int nums)
{
    if (nums == 1)
        return (ft_split(first_number(argc, argv), ' '));  /* "5 4 3" → words   */
    return (dup_tokens(argc, argv, nums));                 /* 5 4 3   → words   */
}
```

Now validate + dedupe into the `int` array. Keep `fill_values` small; on any bad
token, bail (the caller frees + errors):

```c
/* 0 = ok, 1 = syntax / overflow / duplicate */
static int  fill_values(char **words, int *values, int n)
{
    int     i;
    int     j;
    int     over;
    long    v;

    i = 0;
    while (i < n)
    {
        over = 0;
        if (error_syntax(words[i]))
            return (1);
        v = ft_atoi_ps(words[i], &over);   /* decoupled version — see §5 */
        if (over)
            return (1);
        j = 0;
        while (j < i)
            if (values[j++] == (int)v)
                return (1);
        values[i++] = (int)v;
    }
    return (0);
}
```

And the public entry the builder + checker both call:

```c
/* returns malloc'd validated int[count]; on any error: frees + Error + exit */
int *parse_values(int argc, char **argv, int nums, int *count)
{
    char    **words;
    int     *values;
    int     n;

    words = collect_words(argc, argv, nums);
    if (!words || !words[0])
        return (free_words(words), write(2, "Error\n", 6), exit(1), NULL);
    n = 0;
    while (words[n])
        n++;
    values = malloc(sizeof(int) * n);
    if (!values || fill_values(words, values, n))
    {
        free_words(words);
        free(values);
        write(2, "Error\n", 6);
        exit(1);
    }
    free_words(words);
    *count = n;
    return (values);
}
```

> `free_words(char **)` is the standard "free each string then the array" helper
> (you already have `free_tokens` in `build_stack.c` — rename/relocate it here so
> both the split and dup_tokens paths share one free).

`values.c` functions: `first_number`, `dup_tokens`, `collect_words`,
`fill_values`, `parse_values` = 5. Put `free_words` in a utils file or fold the
loop — mind the Norm count.

---

## 4. Phase 3 — build the stack from values (the "builder")

Now the builder is *pure construction*. No validation, no argv, no flags — it just
turns clean ints into nodes. This is the function the checker reuses verbatim.

```c
void build_from_values(t_stack_check *a, int *values, int count)
{
    t_node  *node;
    int     i;

    i = 0;
    while (i < count)
    {
        node = node_creator(values[i]);
        if (!node)
        {
            free_stack(a);
            free(values);
            write(2, "Error\n", 6);
            exit(1);
        }
        node_connecter(a, node);
        i++;
    }
}
```

Your existing `build_stack`, `add_value`, `parse_split` in `build_stack.c` can now
be **deleted** — `parse_values` + `build_from_values` replace them. `error_syntax`,
`error_duplicate` move into the number logic (dup is now the inner loop in
`fill_values`, so `error_duplicate` can go).

---

## 5. Decouple `ft_atoi_ps` from the stack

Today `ft_atoi_ps` takes the stack + argv + `is_split` only so it can call
`free_and_exit` on overflow. In the new design, atoi should just **report**
overflow and let the caller clean up. This is what makes it reusable:

```c
/* old: long ft_atoi_ps(char *str, t_stack_check *stack, char **argv, bool is_split)
   new: */
long    ft_atoi_ps(char *str, int *overflow)
{
    long    res;
    int     sign;
    int     i;

    res = 0;
    sign = 1;
    i = 0;
    if (str[i] == '-' || str[i] == '+')
        if (str[i++] == '-')
            sign = -1;
    while (str[i])
    {
        res = res * 10 + (str[i++] - '0');
        if (res * sign > 2147483647 || res * sign < -2147483648)
        {
            *overflow = 1;
            return (0);
        }
    }
    return (res * sign);
}
```

No stack, no argv, no exit — a plain string→int with an out-param for the one
failure mode. `free_and_exit` can be deleted entirely (its callers now handle
their own cleanup: `parse_values` frees words+values, `build_from_values` frees the
stack).

---

## 6. `main` as the conductor

```c
int main(int argc, char **argv)
{
    t_opts          opts;
    t_stack_check   *a;
    t_stack_check   *b;
    int             *values;
    int             count;

    if (argc < 2)
        return (0);
    count = 0;
    if (read_opts(&opts, argc, argv) == 0)   /* only flags, no numbers */
        return (0);                          /* nothing to sort, exit 0 */
    a = init_stack();
    b = init_stack();
    if (!a || !b)
        return (write(2, "Error\n", 6), 1);
    values = parse_values(argc, argv, count_from_read_opts, &count);
    build_from_values(a, values, count);
    free(values);
    a->disorder = compute_disorder(a);
    if (!is_sorted(a))
        sort_dispatch(a, b, &opts);
    if (opts.bench)
        print_benchmark_summary(a, b);
    free_stack(a); free_stack(b); free(a); free(b);
    return (0);
}
```

> Capture `read_opts`'s return value into `nums` and pass it to `parse_values`
> (shown loosely above as `count_from_read_opts`). `main` reads long here for
> clarity — in your repo, hoist `parse_values`+cleanup into a helper to satisfy the
> 25-line Norm limit.

`sort_dispatch` now takes `opts` instead of reading `a->strategy`:

```c
static void sort_dispatch(t_stack_check *a, t_stack_check *b, t_opts *opts)
{
    if (a->size == 2)
        sa(a);
    else if (a->size == 3)
        three_sorter(a);
    else if (opts->strategy == SIMPLE)
        simple_sorter(a, b);
    else if (opts->strategy == MEDIUM)
        medium_sorter(a, b);
    else
        complex_sorter(a, b);   /* COMPLEX + ADAPTIVE default */
}
```

Bench stays as-is but is gated by `opts.bench` and reads the per-stack counters
(`a->op_*`, `b->op_*`).

---

## 7. Why this is the right shape for the bonus (`checker`)

The checker needs stack `a` built from the **same** `argv` numbers, then it reads
operations from stdin. With the split above, `checker/main.c` reuses the number
pipeline directly and compiles **none** of the flag code:

```c
/* checker main (sketch) */
values = parse_values(argc, argv, nums, &count);  /* SAME function  */
build_from_values(a, values, count);              /* SAME function  */
free(values);
read_and_apply_ops(a, b);                          /* NEW checker-only code */
if (is_sorted(a) && !b->size) write(1, "OK\n", 3);
else                          write(1, "KO\n", 3);
```

Because `parse_values`/`build_from_values`/`ft_atoi_ps`/`error_syntax` live in
`values.c` with **no** reference to `t_opts`, op counters, or sorting, the checker
links them clean. That's the concrete, defensible payoff — *not* raw speed.

What is **not** shared (be clear on this at eval): reading `sa\npb\n...` from
stdin is a separate parser the checker writes from scratch. Separating the number
parser doesn't shortcut that.

---

## 8. File / Norm summary

| File | Change |
|------|--------|
| `push_swap.h` | add `t_strategy` enum + `t_opts`; prototypes for `read_opts`, `is_flag`, `first_number`, `parse_values`, `build_from_values`, `medium_sorter`; change `ft_atoi_ps` signature; drop `build_stack`/`add_value`/`free_and_exit`/`error_duplicate` protos |
| `flags.c` (new) | `ft_strcmp`(s), `is_flag`, `set_opt`(s), `read_opts` |
| `values.c` (new) | `first_number`, `dup_tokens`(s), `collect_words`(s), `fill_values`(s), `parse_values` |
| `build_stack.c` | keep `init_stack`, `node`-building helpers; replace body with `build_from_values`; delete `add_value`/`parse_split`/`build_stack` |
| `parser.c` | slim `ft_atoi_ps` to `(str, *overflow)`; keep `error_syntax`; delete `error_duplicate`/`free_and_exit` |
| `main.c` | new conductor + `sort_dispatch(a, b, &opts)` + `if (opts.bench)` |
| `Makefile` | add `flags.c`, `values.c`, **and `medium.c`** (currently missing) |

Watch the ≤5-functions-per-file and ≤25-lines-per-function limits when you place
`free_words` and the `main` helper.

---

## 9. Verify (same behaviour as Step 10, cleaner internals)

```sh
./push_swap --complex 5 4 3 2 1 | ./checker 5 4 3 2 1     # OK
./push_swap 5 4 3 2 1 --bench    | ./checker 5 4 3 2 1     # OK
./push_swap --bench "5 4 3 2 1"                            # sorts + bench
./push_swap --simple ; echo $?                            # exit 0, no output
./push_swap --turbo 3 2 1                                  # Error
./push_swap "" ; echo $?                                   # Error, 1
```

---

### TL;DR
- **Move `strategy`/`bench` into a `t_opts` struct** — off the stack. `b` stays clean. ✅ big win, tiny cost.
- **Parse flags first (`read_opts`), then numbers (`parse_values`), then build (`build_from_values`).** Clear phases, checker-reusable number path. ✅
- **Leave op counters on the stack** (per-stack, teammate's area). 
- It's **cleaner and more reusable, not faster.** Say that, not "more efficient," at eval.
