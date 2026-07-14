# 02 — The Parsing Pipeline

Turning `argv` into a validated stack A. Four files cooperate:
`flags.c` (separate flags from numbers) → `build_stack.c` (orchestrate) →
`parser.c` (validate one number) → `ft_split.c` (break a quoted `"5 4 3"` apart).

Entry point from `main`: `build_stack(a, argc, argv)`.

---

## A. `flags.c` — classify flags vs numbers

### `ft_strcmp` (lines 15–23) — private helper

```c
static int  ft_strcmp(char *a, char *b)
{
    int i;

    i = 0;
    while (a[i] && a[i] == b[i])   // advance while chars match and a isn't over
        i++;
    return ((unsigned char)a[i] - (unsigned char)b[i]);  // 0 = equal
}
```

Standard `strcmp`. `static` = visible only in this file. The `(unsigned char)`
cast makes the difference well-defined. We only care whether it returns `0`
(strings equal). Used to match a token against each flag name.

### `is_flag_token` (lines 28–31)

```c
int is_flag_token(char *arg)
{
    return (arg[0] == '-' && arg[1] == '-');  // starts with "--"
}
```

The whole flag-vs-number decision in one line: a **flag** starts with `--`. A
**negative number** like `-42` starts with a *single* `-`, so `arg[1]` is `'4'`,
not `'-'` → correctly treated as a number. Safe on `""` (empty string): `arg[0]`
is `'\0'`, the `&&` short-circuits, returns false.

### `apply_flag` (lines 35–49) — private

```c
static void apply_flag(t_stack *a, char *arg)
{
    if (!ft_strcmp(arg, "--bench"))         a->bench = true;
    else if (!ft_strcmp(arg, "--simple"))   a->strategy = SIMPLE;
    else if (!ft_strcmp(arg, "--medium"))   a->strategy = MEDIUM;
    else if (!ft_strcmp(arg, "--complex"))  a->strategy = COMPLEX;
    else if (!ft_strcmp(arg, "--adaptive")) a->strategy = ADAPTIVE;
    else                                    free_and_exit(a, NULL, false);
}
```

Given a token we already know is a flag, set the corresponding state. `!ft_strcmp`
reads as "if equal." `--bench` sets the boolean; the four selectors set the enum.
An **unknown** `--xyz` is a usage error → `free_and_exit` prints `Error` and exits.

### `parse_flags` (lines 52–68) — the public one-pass classifier

```c
int parse_flags(t_stack *a, int argc, char **argv)
{
    int i;
    int nums;

    i = 1;                          // skip argv[0] (program name)
    nums = 0;
    while (i < argc)
    {
        if (is_flag_token(argv[i]))
            apply_flag(a, argv[i]);  // it's a flag → set state
        else
            nums++;                  // it's a number → just count it here
        i++;
    }
    return (nums);                   // how many NUMBER arguments there are
}
```

One walk over `argv`. Flags are applied immediately; numbers are only **counted**
(they get validated later). The return value (`nums`) is what lets `build_stack`
decide the input *shape* — see below.

### `first_number` (lines 71–83)

```c
char *first_number(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc)
    {
        if (!is_flag_token(argv[i]))
            return (argv[i]);   // first non-flag token
        i++;
    }
    return (NULL);
}
```

Returns the first argument that isn't a flag. Used for the single-argument
`"5 4 3"` case, where the numbers all live inside one quoted string that we must
then `ft_split`.

---

## B. `build_stack.c` — orchestrate parsing + build the list

### `init_stack` (lines 15–40)

```c
t_stack *init_stack(void)
{
    t_stack *stack;

    stack = (t_stack *)malloc(sizeof(t_stack));
    if (!stack)
        return (NULL);
    stack->head = NULL;  stack->tail = NULL;
    stack->max = NULL;   stack->min = NULL;
    stack->size = 0;     stack->disorder = 0.0;
    stack->op_sa = 0;    ...  stack->op_rrr = 0;   // all 11 counters → 0
    return (stack);
}
```

Allocates a stack and zeroes **every** field so nothing is read uninitialised.
Called twice in `main` (for A and B). Returns `NULL` on malloc failure, which
`main` checks. (Note: `strategy`/`bench` are *not* set here — they're set later by
`parse_flags`; `strategy` happens to default correctly only if the flag runs. In
practice `parse_flags` always runs before use. A defensive improvement would be to
zero them here too.)

### `free_tokens` (lines 41–52) — private

```c
static void free_tokens(char **arr)
{
    int i = 0;
    while (arr[i]) { free(arr[i]); i++; }   // free each string
    free(arr);                              // then the array itself
}
```

Frees a `char **` produced by `ft_split` (the split case owns its strings).

### `add_value` (lines 54–68) — validate ONE token and append it

```c
static void add_value(t_stack *a, char *tok, char **arr, bool is_split)
{
    int     value;
    t_node  *node;

    if (error_syntax(tok))                       // 1. is it a valid integer text?
        free_and_exit(a, arr, is_split);
    value = (int)ft_atoi_ps(tok, a, arr, is_split); // 2. convert (checks overflow)
    if (error_duplicate(a, value))               // 3. seen this value already?
        free_and_exit(a, arr, is_split);
    node = node_creator(value);                  // 4. make a node
    if (!node)
        free_and_exit(a, arr, is_split);
    node_connecter(a, node);                     // 5. append to the list
}
```

The four validation gates the subject requires — bad syntax, overflow, duplicate,
malloc failure — each routes to `free_and_exit`. `arr`/`is_split` are passed
through only so the cleanup can free the split array when needed.

### `parse_split` (lines 70–85) — the `"5 4 3"` case

```c
static void parse_split(t_stack *a, char *arg)
{
    char    **arr;
    int     i;

    arr = ft_split(arg, ' ');           // "5 4 3" → ["5","4","3",NULL]
    if (!arr)
        free_and_exit(a, NULL, false);
    i = 0;
    while (arr[i])
    {
        add_value(a, arr[i], arr, true); // is_split=true → cleanup frees arr
        i++;
    }
    free_tokens(arr);                    // success path also frees arr
}
```

Splits one quoted string on spaces, validates each word. `is_split = true` tells
`add_value`/`free_and_exit` that on error they must free `arr`.

### `build_stack` (lines 87–109) — the orchestrator

```c
void build_stack(t_stack *a, int argc, char **argv)
{
    int i;
    int nums;

    nums = parse_flags(a, argc, argv);   // apply flags, count number args
    if (nums == 0)
        return ;                         // only flags given → nothing to sort
    if (nums == 1)
        parse_split(a, first_number(argc, argv));  // one arg → quoted list
    else                                            // many args → each is a number
    {
        i = 1;
        while (i < argc)
        {
            if (!is_flag_token(argv[i]))
                add_value(a, argv[i], NULL, false); // is_split=false (argv-owned)
            i++;
        }
    }
    if (a->size == 0)
        free_and_exit(a, NULL, false);   // e.g. ./push_swap "" → Error
}
```

This is the crux of how flags were bolted on. Read the three exits carefully —
they encode subject-correct edge behaviour:

- **`nums == 0`** → only flags were passed (`./push_swap --bench`). Return with an
  empty stack; `main` prints nothing and exits 0. **Not** an error.
- **`nums == 1`** → the numbers are inside one quoted string → `parse_split`.
- **`nums >= 2`** → each non-flag `argv[i]` is its own number; flags are skipped by
  the `is_flag_token` guard.
- **`a->size == 0` after parsing** → we *tried* to read numbers but got none
  (e.g. `./push_swap ""` → `ft_split` yields an empty list) → `Error`.

> **Why `nums` and not the old `argc == 2`?** Because a flag occupies an `argv`
> slot. `./push_swap --bench "5 4 3"` has `argc == 3` but is still the
> single-quoted-list case. Counting **non-flag** args makes the shape decision
> independent of how many flags surround the numbers, and independent of flag
> position.

---

## C. `parser.c` — validate & convert a single number

### `error_syntax` (lines 15–33)

```c
int error_syntax(char *str)
{
    int i = 0;
    if (!str || !str[i])                 // NULL or empty → invalid
        return (1);
    if (str[i] == '-' || str[i] == '+')  // optional leading sign
        i++;
    if (!str[i])                         // just "-" or "+" → invalid
        return (1);
    while (str[i])                       // every remaining char must be a digit
    {
        if (str[i] < '0' || str[i] > '9')
            return (1);
        i++;
    }
    return (0);                          // 0 = valid integer text
}
```

Accepts an optional single `+`/`-` then one-or-more digits. Rejects empty, a lone
sign, and anything with a non-digit (letters, a second sign, spaces, `.`).
Returns 1 on error to match the `if (error_...) → fail` pattern.

### `error_duplicate` (lines 35–49)

```c
int error_duplicate(t_stack *stack, int dup)
{
    t_node *curr;
    if (!stack || !stack->head)   // empty stack → nothing duplicated yet
        return (0);
    curr = stack->head;
    while (curr)
    {
        if (curr->value == dup)   // value already present?
            return (1);
        curr = curr->next;
    }
    return (0);
}
```

Linear scan of the values already inserted. Because it's called for every new
value, building the stack is **O(n²)** overall — fine for n up to a few thousand.

### `ft_atoi_ps` (lines 51–76) — atoi with overflow guard

```c
long ft_atoi_ps(char *str, t_stack *stack, char **argv, bool is_split)
{
    long res = 0;
    int  sign = 1;
    int  i = 0;

    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-') { sign = -1; i++; }   // NOTE: '+' is not skipped here
    }
    while (str[i])
    {
        res = (res * 10) + (str[i] - '0');       // build the number digit by digit
        if (res * sign > 2147483647 || res * sign < -2147483648)
            free_and_exit(stack, argv, is_split); // out of int range → Error
        i++;
    }
    return (res * sign);
}
```

Converts text to a `long`, checking after **every digit** that it stays within
`INT_MIN..INT_MAX`. Using `long` for `res` means the check fires long before a
64-bit overflow could happen. On overflow it cleans up and exits.

- `INT_MIN` (`-2147483648`) is accepted: `res*sign` equals exactly the bound, and
  the test is strict `<`.
- Small subtlety: on a leading `'+'` the inner `if` doesn't advance `i`, so the
  first loop iteration reads `'+'`... but `error_syntax` already guaranteed the
  first char after a sign is a digit, and this function is only called *after*
  `error_syntax` passed. In practice `'+'` inputs are handled because
  `error_syntax` allows the sign and the digits follow. (A cleaner version would
  `i++` on `'+'` too; worth mentioning if the evaluator probes `+5`.)

### `free_and_exit` (lines 78–96) — the single error exit

```c
void free_and_exit(t_stack *stack, char **argv, bool is_split)
{
    int i;
    if (argv && is_split)             // only the split path owns argv strings
    {
        i = 0;
        while (argv[i]) { free(argv[i]); i++; }
        free(argv);
    }
    if (stack)
        free_stack(stack);            // free the nodes built so far
    write(2, "Error\n", 6);           // subject: exactly "Error\n" to stderr
    exit(1);
}
```

Central cleanup + `Error`. `is_split` distinguishes "these strings came from
`ft_split` (we own them)" from "these are `argv` pointers (the OS owns them)."

> **Known leak (minor):** this frees only `stack` (A). Stack **B**, allocated in
> `main` before `build_stack`, is not freed on this path. The process exits
> immediately so the OS reclaims it, but `valgrind` on an *invalid* input will
> report B as still reachable. Note it in `07`.

---

## D. `ft_split.c` — split a string on a delimiter

A from-scratch `split` (libft not linked). Splits `"5 4 3"` into `["5","4","3"]`.

- **`element_counter` (15–34):** counts how many words exist by tracking an
  `in_element` flag — increments `count` each time we transition from delimiter to
  non-delimiter. Tells us how big the result array must be.
- **`free_all` (36–41):** frees the first `i` allocated words then the array — used
  when an allocation fails midway.
- **`allocate_element` (43–59):** `malloc(len+1)`, copy `len` chars, NUL-terminate.
  Produces one word.
- **`fill_the_res` (61–88):** walks the string; at each word start, measures its
  length, allocates+copies it, advances past it. NULL-terminates the array.
  Returns 0 (after `free_all`) on allocation failure, 1 on success.
- **`ft_split` (90–102):** guards `NULL`, allocates the array
  (`element_counter + 1` for the NULL terminator), fills it, returns it.

> Small note: on the `fill_the_res` failure path, `ft_split` returns `NULL` but the
> array was already freed inside `free_all` — consistent. On the outer `malloc`
> failure it returns `NULL` too. Both are handled by `parse_split`'s `if (!arr)`.

➡ Next: `03_stack_and_operations.md`
