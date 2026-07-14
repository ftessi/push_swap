# 06 — Program Lifecycle & Dispatch (`main.c`)

`main.c` is the conductor: set up, parse, measure, route to a strategy, optionally
print benchmarks, clean up. It also holds `sort_dispatch` (strategy routing) and
`print_benchmark_summary` (the `--bench` output).

---

## A. `print_benchmark_summary` (lines 5–26)

```c
void print_benchmark_summary(t_stack *a, t_stack *b)
{
    int total_ops;

    if (!a || !b)
        return ;
    total_ops = a->op_sa + b->op_sb + a->op_ss + a->op_pa + b->op_pb + a->op_ra
        + b->op_rb + a->op_rr + a->op_rra + b->op_rrb + a->op_rrr;
    printf("\n ================== OPS SUMMARY ==================\n");
    printf("SWAPS    | SA : %d\t| SB : %d\t| SS : %d\n", a->op_sa, b->op_sb, a->op_ss);
    printf("PUSHES   | PA : %d\t| PB : %d\n", a->op_pa, b->op_pb);
    printf("ROTATES  | RA : %d\t| RB : %d\t| RR : %d\n", a->op_ra, b->op_rb, a->op_rr);
    printf("REV-ROT  | RRA: %d\t| RRB: %d\t| RRR: %d\n", a->op_rra, b->op_rrb, a->op_rrr);
    printf("--------------------------------------------------\n");
    printf("METRICS  | Initial Disorder: %.2f\n", a->disorder);
    printf("--------------------------------------------------\n");
    printf(" TOTAL MOVES: %d\n", total_ops);
    printf("==================================================\n\n");
}
```

Sums the eleven counters into `total_ops` and prints a formatted table plus the
disorder. Note **where each counter lives**: swaps/pushes/rotates that act on A are
read from `a` (`a->op_sa`, `a->op_pa`, `a->op_ra`, `a->op_rr`, `a->op_rra`,
`a->op_rrr`, `a->op_ss`) and the B-side ones from `b` (`b->op_sb`, `b->op_pb`,
`b->op_rb`, `b->op_rrb`) — matching which stack each op incremented in the `ops_*`
wrappers. `%.2f` prints disorder as e.g. `0.50`.

> ⚠️ **Two defense risks live in this function** (detail in `07`):
> 1. It uses **`printf`** (and `main.c` includes `<stdio.h>`) — not an allowed
>    function for the mandatory part.
> 2. It writes to **stdout**, so if you ever pipe `--bench` output into the checker,
>    these lines pollute the op stream. The subject-style approach is to print the
>    stats to **stderr** with your own `write`/`ft_printf`. It's gated behind
>    `--bench`, so normal `./push_swap $ARG | ./checker $ARG` runs are unaffected.

---

## B. `sort_dispatch` (lines 28–43) — choosing the strategy

```c
static void sort_dispatch(t_stack *a, t_stack *b)
{
    if (a->size == 2)
        sa(a);                          // 2 elements: at most one swap
    else if (a->size == 3)
        three_sorter(a);                // 3 elements: the base case
    else if (a->size > 3)
    {
        if (a->disorder < 0.20)
            simple_sorter(a, b);        // nearly sorted → insertion
        else if (a->disorder >= 0.20 && a->disorder < 0.50)
            medium_sorter(a, b);        // moderately scrambled → chunks
        else
            complex_sorter(a, b);       // very scrambled → cheapest-cost
    }
}
```

Routing:
- **size 2** → a single `sa` if needed (dispatch is only reached when *not* already
  sorted, so the swap is always the fix).
- **size 3** → `three_sorter`.
- **size > 3** → pick by **disorder**: `<0.20` simple, `[0.20,0.50)` medium, `≥0.50`
  complex. The thresholds encode "cheap method for tidy input, heavy method for
  random input."

> 🔴 **The most important honesty point in the whole project:** this function
> **never reads `a->strategy`.** The `--simple/--medium/--complex/--adaptive` flags
> are parsed and stored, but selection is *always* by disorder. So:
> - `./push_swap --simple <very random input>` will actually run **complex**.
> - The flags therefore behave like a no-op for algorithm choice (only `--bench`
>   does something observable).
>
> If an evaluator asks "what does `--simple` do?", the truthful answer is: "it sets
> a field, but our dispatcher currently auto-selects by disorder and ignores that
> field — so in this build the selector flags don't change behavior. To honor them
> we'd branch on `a->strategy` first and fall back to disorder for `ADAPTIVE`." (A
> ~5-line fix — see `07`.)

---

## C. `main` (lines 45–74)

```c
int main(int argc, char **argv)
{
    t_stack *a;
    t_stack *b;

    if (argc < 2)                       // no arguments at all
        return (0);                     // print nothing, exit 0
    a = init_stack();
    b = init_stack();
    if (!a || !b)                       // malloc failure
    {
        write(2, "Error\n", 6);
        return (1);
    }
    build_stack(a, argc, argv);         // parse flags + numbers into A
    if (a->size > 0)                    // skip everything if only flags/no numbers
    {
        a->disorder = compute_disorder(a);   // measure BEFORE moving anything
        if (!is_sorted(a))              // already sorted → emit no ops
            sort_dispatch(a, b);
    }
    if (a->bench)                       // optional stats block
        print_benchmark_summary(a, b);
    free_stack(a);                      // free nodes of A and B
    free_stack(b);
    free(a);                            // free the two structs
    free(b);
    return (0);
}
```

The full lifecycle, step by step:

1. **`argc < 2`** → nothing to do; exit 0 with no output (subject rule).
2. **Allocate** A and B; on malloc failure, `Error` to stderr and exit 1.
3. **`build_stack`** parses flags into A and builds the number list (doc `02`). On
   any invalid input it never returns — it `free_and_exit`s.
4. **`if (a->size > 0)`** guards the case where only flags were passed
   (`./push_swap --bench`): A is empty, so we skip measuring/sorting but still fall
   through to the bench print.
5. **`compute_disorder`** runs *before* any operation, so the reported/used disorder
   reflects the original input.
6. **`is_sorted`** short-circuits already-sorted input → zero ops emitted.
7. **`sort_dispatch`** emits the operation stream to stdout.
8. **`--bench`** optionally prints the summary.
9. **Cleanup**: free the nodes (`free_stack`) then the structs (`free`) for both
   stacks, return 0.

> Lines 53 (a leftover Turkish comment) and the general inline commenting are
> Norm issues — see `07`. Behaviorally the function is correct.

### Edge-case behavior summary (good to recite at defense)

| Input | What happens |
|-------|--------------|
| `./push_swap` | `argc < 2` → exit 0, no output |
| `./push_swap 42` | one number, already "sorted" → no ops, exit 0 |
| `./push_swap 1 2 3` | `is_sorted` true → no ops, exit 0 |
| `./push_swap 3 2 1` | dispatch by disorder → ops printed |
| `./push_swap "3 2 1"` | split path → same as above |
| `./push_swap --bench 3 2 1` | ops printed + stats block |
| `./push_swap --simple` | only a flag → `nums==0`, empty A → no output, exit 0 |
| `./push_swap 1 2 two` | `error_syntax` fails → `Error`, exit 1 |
| `./push_swap 1 2 2` | `error_duplicate` → `Error`, exit 1 |
| `./push_swap 9999999999` | overflow in `ft_atoi_ps` → `Error`, exit 1 |
| `./push_swap ""` | `nums==1`, split gives 0 values, `size==0` → `Error`, exit 1 |
| `./push_swap --xyz 1 2` | unknown flag → `Error`, exit 1 |

➡ Next: `07_defense_qa_and_risks.md`
