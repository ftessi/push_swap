# Step 04 — Choosing the Data Structure

> **Goal:** Decide how `a` and `b` live in memory. This is the most consequential
> design decision in the project — make it together, write it down, and don't
> change it casually later.

## The core tension

The operations describe a stack, but `rra`/`rrb` touch the **bottom**, and your
algorithms will constantly ask "how big is the stack?", "what's the max?", "what
position is value X?". So you need fast access to **both ends** and to **size**.

There are two mainstream choices. Understand both, then pick one.

### Option A — Doubly linked list
Each node holds a value and `next`/`prev` pointers; you keep a pointer to the
top (and ideally the bottom, or a size counter).

- **Pro:** push/pop/rotate are pure pointer surgery — elegant, no shifting.
- **Pro:** matches the "stack" mental model closely.
- **Con:** lots of malloc/free → more leak surface; you *must* be disciplined.
- **Con:** "what's at index i" or "find max" means walking the list (O(n)).

### Option B — Array + index, or array-based stack
Represent each stack as an array plus a count (and decide which end is "top").

- **Pro:** no per-element malloc; one allocation, trivially leak-safe.
- **Pro:** random access is O(1) — handy for some analysis.
- **Con:** `pa`/`pb`/rotate may mean shifting elements (O(n) memcpy) unless you're
  clever with a head index / circular buffer.
- **Con:** you carry a fixed capacity around.

> Either passes. Many strong push_swap solutions use a **doubly linked list**.
> Many fast radix solutions use **arrays** because radix wants index access. Pick
> the one *both of you* are comfortable defending. If unsure, a doubly linked list
> with a stored size is the most classic and the easiest to reason about.

## The "no globals" consequence: a context struct

Since globals are forbidden, wrap everything you need to pass around in one struct
and hand a pointer to it. Think about what every function needs:

- A handle to stack `a` and stack `b` (and their sizes).
- The chosen strategy (an enum: SIMPLE/MEDIUM/COMPLEX/ADAPTIVE).
- A flag for `--bench`.
- If benchmarking: a per-operation counter table (one count per op type).

Sketch (shape only — you fill in the real types):

```
typedef struct s_stack { /* your node-or-array representation + size */ } t_stack;

typedef struct s_ctx {
    t_stack a;
    t_stack b;
    int     strategy;     // enum
    int     bench;        // 0/1
    long    op_count[11]; // sa,sb,ss,pa,pb,ra,rb,rr,rra,rrb,rrr
    double  disorder;     // computed before sorting
} t_ctx;
```

This struct is the spine of the whole project. Designing it well now means your
operation functions can bump `op_count` for free (Step 13), and you never need a
global.

## The single most useful trick: index normalization

Your inputs can be **negative, huge, or sparse** (e.g. `-2147483648`, `42`,
`1000000`). Comparing raw values works for the simple sort, but the medium and
complex strategies become *much* easier if you first replace each value by its
**rank**: its position if the values were sorted, `0 .. n-1`.

Example: `[42, -7, 1000, 3]` → ranks `[2, 0, 3, 1]`.

Why this is powerful:
- Negatives and huge ranges disappear; you only deal with `0 .. n-1`.
- **Radix sort** (Step 10) needs exactly this — it sorts by the *bits of the
  index*.
- **Chunk sort** (Step 09) can split the range `0 .. n-1` into clean √n chunks.
- "Is it sorted?" becomes "is it `0,1,2,...,n-1`?".

You can either store both the original value and its index per element, or sort
purely on the index once parsing is done (the original values are irrelevant to
the *operations* you output — only their relative order matters). Decide and be
consistent.

> The **disorder metric** (Step 07) compares relative order too, so it can use raw
> values or ranks — same result, since ranks preserve order. Just be consistent.

## Manual: build and test the structure

1. Define the struct(s) in `push_swap.h` with your teammate.
2. Write tiny helpers: create/free a stack, get size, peek top, peek bottom,
   `is_sorted(a)`, `stack_min`, `stack_max`. These live in `stack_utils.c`.
3. Write a debug-only print function (NOT submitted in final output path) that
   dumps `a` and `b` side by side. You will use this constantly. Gate it so it's
   never called in graded runs.
4. Free everything and run under valgrind. Zero leaks before you build anything on
   top of this.

## Verify
- Create a stack from a known list, print it, confirm order (first arg = top).
- `is_sorted` returns true for `0..n-1` and false otherwise.
- valgrind clean on create + free.

➡ Next: `05_parsing_and_errors.md`
