# 01 — Data Structures (`push_swap.h`)

Everything in the project is built on three types defined in the header. If you
understand these, the rest of the code reads naturally.

---

## 1. `t_node` — one element of a stack (lines 20–30)

```c
typedef struct s_node
{
    int             value;    // the actual integer the user gave us
    int             index;    // "rank" 0..n-1 = how many values are smaller
    int             pos;      // current position from the top (0 = top)
    bool            median;   // true if this node is in the TOP half of the stack
    int             cost;     // (complex sort) total rotations to move this node
    struct s_node   *target;  // (complex sort) where this node should land
    struct s_node   *next;    // node below this one
    struct s_node   *prev;    // node above this one
}                   t_node;
```

A stack is a **doubly-linked list** of these. Why doubly linked? Because the moves
work at *both* ends: `ra`/`pb` touch the **top** (`head`), while `rra`/`rrb` touch
the **bottom** (`tail`). Having `prev` as well as `next` lets us detach the tail in
O(1) instead of walking the whole list.

Field-by-field, and *who fills it*:

- `value` — set once at creation (`node_creator`), never changes.
- `index` — a **rank**: how many elements are smaller than this one. Filled by
  `assign_rank` in `medium.c`. Turns arbitrary ints (e.g. `-2 000 000`, `42`) into
  a clean `0..n-1` scale so we can chunk them.
- `pos` — distance from the top, recomputed every time we call `stack_o_meter`.
  `pos == 0` means "currently on top." Used everywhere to decide rotation counts.
- `median` — set in `stack_o_meter` as `pos <= size/2`. It answers "is this node
  nearer the top or the bottom?" so a sorter can choose between rotating up (`ra`)
  or down (`rra`) — whichever is fewer moves.
- `cost` — only the **complex** sorter uses it: the number of operations required
  to bring this B-node (and its A-target) into place. Filled by `assign_cost`.
- `target` — only complex: a pointer to the node in A this element should sit on
  top of. Filled by `assign_cost` via `target_the_a`.
- `next` / `prev` — the list wiring, maintained by every operation.

> **Defense note:** a node carries fields used by *different* strategies (`index`
> for medium, `cost`/`target` for complex). They're all zero-initialised in
> `node_creator`, so an unused field is simply never read. This keeps one node type
> for all strategies instead of three.

---

## 2. `t_strategy` — the algorithm selector enum (lines 32–38)

```c
typedef enum e_strategy
{
    ADAPTIVE,   // = 0, the default
    SIMPLE,     // = 1
    MEDIUM,     // = 2
    COMPLEX     // = 3
}   t_strategy;
```

An `enum` is just named integers. `ADAPTIVE` is `0`, so a freshly-initialised
stack defaults to `ADAPTIVE` "for free." A flag like `--simple` sets the field to
`SIMPLE`.

> **Honesty flag:** this enum is *set* by `parse_flags` but the dispatcher in
> `main.c` currently ignores it and chooses by disorder instead. See `06` and `07`.

---

## 3. `t_stack` — a whole stack (lines 40–61)

```c
typedef struct s_stack_check
{
    t_node      *head;      // top element
    t_node      *tail;      // bottom element
    t_node      *max;       // node with the largest value (cached)
    t_node      *min;       // node with the smallest value (cached)
    int         size;       // element count
    double      disorder;   // fraction [0,1] of out-of-order pairs
    int         op_sa;      // ── the 11 operation counters ──
    int         op_sb;
    int         op_ss;
    int         op_pa;
    int         op_pb;
    int         op_ra;
    int         op_rb;
    int         op_rr;
    int         op_rra;
    int         op_rrb;
    int         op_rrr;
    t_strategy  strategy;   // which algorithm was requested
    bool        bench;      // print the stats block?
}               t_stack;
```

Note the typedef name is `t_stack` even though the struct tag is
`s_stack_check` — throughout the code we write `t_stack`.

Why keep `head` **and** `tail`? O(1) access to both ends (needed by rotate vs
reverse-rotate). Why cache `max`/`min`? So `three_sorter` and the final alignment
can find extremes without re-scanning; they're refreshed by `stack_o_meter`.

`disorder` is a `double` in `[0,1]` — e.g. `0.4993` means 49.93% of all value
pairs are in the wrong order. Computed once, before sorting, in `compute_disorder`.

The eleven `op_*` counters are incremented inside each operation function
(`sa` does `a->op_sa++`, etc.). `--bench` sums them to report total moves. They
live on the stack because each stack tallies the moves that touched *it*.

`strategy` and `bench` are program *options* parsed from flags. (Architecturally
they'd be cleaner in a separate options struct — see
`session_analysis_2026-07-01/11_flags_architecture_refactor.md` — but the team
chose to keep them on the stack to ship. Be ready to say why: it's convenient
because the stack pointer is already passed everywhere.)

---

## 4. The prototype sections (lines 63–116)

The header groups prototypes by role, which mirrors this folder:

- **FLAGS** (63–66): `is_flag_token`, `parse_flags`, `first_number` → doc `02`.
- **SPLIT & PARSING** (68–75): `ft_split`, `error_syntax`, `error_duplicate`,
  `ft_atoi_ps`, `free_and_exit`, `build_stack` → doc `02`.
- **STACK UTILS** (77–86): node/stack lifecycle, `is_sorted`, `compute_disorder`,
  targeting, `init_stack` → docs `03`/`04`.
- **ALGORITHMS AND UTILS** (88–95): `three_sorter`, the three sorters,
  `assign_cost`, `find_cheapest`, `signed_rot` → docs `04`/`05`.
- **INSTRUCTIONS** (97–115): the low-level movers (`stack_pusher`,
  `stack_swapper`, `stack_rotater`, `stack_rev_rotater`) and the 11 public ops →
  doc `03`.

### Include guard & headers (13–18)

```c
#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <stdbool.h>   // for the bool type (median, bench)
# include <stdlib.h>    // malloc, free, exit
# include <unistd.h>    // write
```

The `#ifndef/#define/#endif` guard stops double-inclusion. Only three system
headers — all in the allowed-functions set. (Note: `main.c` and `simple.c`
additionally include `<stdio.h>`, which is *not* allowed for the mandatory part —
see `07`.)

➡ Next: `02_parsing_pipeline.md`
