# Step 06 — The 11 Operations

> **Goal:** Implement all eleven moves correctly, have each one **print its name**
> to stdout and **count itself** for `--bench`. These are the only verbs your
> algorithms can use; if one is subtly wrong, every algorithm silently breaks.

## The design decision that saves you later

Each operation should do **three things in one place**:
1. Mutate the stack(s) correctly.
2. Print the op name + `\n` to stdout — *unless* you're in checker mode (the bonus
   checker executes ops but doesn't print them). So make printing conditional, or
   separate "apply" from "emit." A clean approach: the op function takes a flag (or
   reads `ctx->bench`/a mode) deciding whether to print.
3. Increment the matching `ctx->op_count[OP]` so `--bench` is free.

If you centralize this, you can never forget to count or print. If you scatter
`write(1,"ra\n",3)` across your algorithms, you *will* desync.

> **Subtlety:** for the main `push_swap` you print every op. For the **bonus
> checker** you execute ops read from stdin and must NOT print them. Same mutation
> logic, different I/O. Structure your op functions so the mutation is reusable by
> both programs (e.g. a pure `do_ra(stack)` plus a thin wrapper that prints+counts).

## Implement each, thinking about edge cases

### Swaps — `sa`, `sb`, `ss`
Exchange the top two elements. **No-op if size < 2.** `ss` = do `sa` and `sb` but
print **one** line `ss` and count it as one `ss` (not as `sa`+`sb`). Watch this:
the bench output counts `ss` separately from `sa`/`sb`.

### Pushes — `pa`, `pb`
Move the top element from one stack to the top of the other. `pb`: top of `a` →
top of `b`. `pa`: top of `b` → top of `a`. **No-op if the source is empty.** These
change both stacks' sizes — update both counts.

### Rotates — `ra`, `rb`, `rr`
Top element goes to the **bottom**; everything shifts up by one. With a doubly
linked list this is "move head to tail"; with an array think carefully about
whether you shift or use a head index. **No-op if size < 2** (rotating 0/1 elements
does nothing). `rr` = `ra` + `rb`, printed/counted as one `rr`.

### Reverse rotates — `rra`, `rrb`, `rrr`
Bottom element goes to the **top**. The inverse of rotate. `rrr` = `rra` + `rrb`,
one line, one count.

## The combined-op rule, stated precisely

`ss`, `rr`, `rrr` are **single operations** in the cost model. The whole reason
they exist is to let you act on both stacks for the price of one. Your algorithms
should *prefer* them whenever they'd otherwise rotate both stacks in the same
direction at the same time (Step 14 is built on this). Implement them as genuine
single ops, not as two prints.

## Manual

1. Write the **pure mutation** functions first (`do_sa`, `do_pb`, ...). Test each
   in isolation with your debug printer from Step 04: set up a known stack, apply
   one op, dump, confirm by hand.
2. Verify the **no-op cases**: `sa` on a 1-element stack changes nothing and
   (decide) still / doesn't print? The classic convention: it still does nothing
   and you simply don't call it on too-small stacks. But if called, it must not
   crash. Most algorithms just won't emit no-op moves.
3. Wrap mutation with the **emit+count** layer.
4. Build `ss`/`rr`/`rrr` on top of the singles, but make sure they emit the
   combined name and bump only the combined counter.

## A correctness test you should automate

For any random stack, the sequence `ra` then `rra` must return it to the original
(they're inverses). Likewise `sa` twice. Likewise `pb` then `pa`. Write a tiny
self-check: apply op + inverse, assert the stack is unchanged. If an inverse pair
doesn't round-trip, that op is buggy — find it now, before it poisons a sort.

## Verify
- Each op matches your hand-trace on a 3–4 element stack.
- Inverse pairs round-trip.
- `ss`/`rr`/`rrr` print one line and update only their own counter.
- No crash when an op is called on an empty/size-1 stack.

➡ Next: `07_disorder_metric.md`
