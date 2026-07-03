# Line-by-line walkthrough of your teammate's code

This explains every source file so **you can defend it** in the evaluation. Where a
line is buggy or non-Norm, it's flagged `⚠️`. Fixes live in `02_bugs_and_fixes.md`.

Sort target throughout the code is **ascending** (smallest value ends on top of
stack A). Keep that in mind — one comment in the code wrongly says "descending".

---

## `push_swap.h` — the shared header

```c
typedef struct s_node
{
    int             value;   // the number the user passed
    int             index;   // rank after normalization (0..n-1); unused so far
    int             pos;     // current position from the top (0 = top); set by stack_o_meter
    bool            median;  // "is this node in the top half?"; set by stack_o_meter
    int             cost;    // rotations needed to move it; for future chunk/radix sort
    struct s_node   *target; // paired node in the other stack; for future sort
    struct s_node   *next;   // node below this one
    struct s_node   *prev;   // node above this one
}   t_node;
```
- A **doubly linked list** node. `next`/`prev` make both rotate and reverse-rotate
  O(1). `index/cost/target/median/pos` are scaffolding for the *later* algorithms
  (they aren't all used yet — that's fine, they're anticipated).

```c
typedef struct s_stack_check
{
    t_node  *head;      // top of the stack
    t_node  *tail;      // bottom of the stack
    t_node  *max;       // cached pointer to the max node (set by stack_o_meter)
    t_node  *min;       // cached pointer to the min node
    int     size;       // number of nodes
    double  disorder;   // 0..1 disorder metric (declared but not computed yet)
}   t_stack_check;
```
- The **container / stack handle**. Holding `tail` and `size` gives O(1) append and
  O(1) size — this is the "option (b)" the readiness review recommended, and the
  earlier `struct s_stack_check *head` bug is now fixed (it correctly points to
  `t_node`). Good.
- ⚠️ Norm: several lines here have trailing spaces / double spaces (`SPC_BEFORE_NL`).

The rest of the header is **function prototypes**, grouped by role (split/parsing,
stack utils, algorithms, and the 11 instructions). This is exactly what the
readiness review asked for.

---

## `stack_utils.c` — building and measuring the stack

### `node_creator(int value)`
```c
node = malloc(sizeof(t_node));
if (!node) return (NULL);
node->value = value; node->index = 0; node->pos = 0;
node->median = false; node->cost = 0;
node->target = NULL; node->next = NULL; node->prev = NULL;
return (node);
```
- Allocates one node and **zero-initializes every field**. Returns `NULL` on malloc
  failure. Correct and clean. (Callers must check for `NULL` — see the bug list.)

### `node_connecter(stack, new_node)` — append to the bottom
```c
if (!stack || !new_node) return ;
if (stack->size == 0) { stack->head = new_node; stack->tail = new_node; }
else {
    stack->tail->next = new_node;
    new_node->prev = stack->tail;
    stack->tail = new_node;
}
stack->size++;
```
- Empty stack → the new node is both head and tail.
- Otherwise → link it after the current tail and move `tail`. `size++` each time.
- **Correct** doubly-linked append. This is what the parser will call per number.

### `stack_o_meter(stack)` — the "scanner"
```c
if (!stack || !stack->head) return ;
stack->max = stack->head;
stack->min = stack->head;
curr = stack->head; i = 0;
while (curr) {
    curr->pos = i;
    curr->median = (i <= (stack->size / 2));   // top half → true
    if (curr->value > stack->max->value) stack->max = curr;
    if (curr->value < stack->min->value) stack->min = curr;
    curr = curr->next; i++;
}
```
- One pass that, for the whole stack: stamps each node's **position** from the top,
  marks whether it's in the **top half** (`median`), and caches pointers to the
  **min** and **max** nodes. `three_sorter` relies on `max->pos`.
- **Correct.** ⚠️ Norm: the `//` comments are inside a function body — forbidden.
- Note: this is *not* the required "disorder metric" (that's a single 0..1 number).
  It's a helper; disorder is still to be written.

### `free_stack(stack)`
```c
if (!stack || !stack->head) return ;
curr = stack->head;
while (curr) { next = curr->next; free(curr); curr = next; }
stack->head = NULL; stack->tail = NULL; stack->size = 0;
```
- Standard list free: save `next` **before** freeing (else use-after-free), then
  reset the container. Correct. ⚠️ It does **not** reset `min`/`max` to NULL — minor,
  but they'd dangle if reused; harmless as-is because `stack_o_meter` re-sets them.

### `is_sorted(stack)`
```c
if (!stack || !stack->head) return (true);
curr = stack->head;
while (curr->next) {
    if (curr->value > curr->next->value) return (false); // ⚠️ comment says "descending" — it's actually ascending
    curr = curr->next;
}
return (true);
```
- Walks top→bottom; if any node is **greater than the one below it**, it's not
  sorted. So "sorted" = **non-decreasing / ascending**. Correct — but the inline
  comment ("cuz descending order") is **wrong and misleading**; fix the comment.

---

## `ft_split.c` — split a string on a delimiter

Classic libft `ft_split`, needed to support the quoted form `"3 1 2"` (one argv
entry with spaces).

- `element_counter(str, c)` — counts words: flips an `in_element` flag on/off as it
  crosses delimiters, `count++` at each word start. Correct.
- `allocate_element(str, len)` — mallocs `len+1`, copies `len` chars, null-terminates.
- `fill_the_res(res, str, c)` — walks the string; at each word measures its length,
  allocates+copies it, advances past it; on any malloc failure calls `free_all` and
  returns 0. Sets `res[i] = NULL` terminator. Correct.
- `free_all(res, i)` — frees the `i` already-allocated words then the array. Good
  cleanup path.
- `ft_split(str, c)` — mallocs `(count+1)` pointers, fills them, returns `res`.
  ⚠️ Small leak-on-failure: if `fill_the_res` fails it already freed the words +
  array inside `free_all`, but `ft_split` then `return (NULL)` — OK. But note it
  does **not** free `res` in the "malloc succeeded, fill failed" case beyond what
  `free_all` did — actually `free_all` frees `res` too, so this is fine.
- ⚠️ Norm: two `MISALIGNED_VAR_DECL` (declaration column alignment).

---

## The 11 operations

### `ops_swap.c` — `stack_swapper` + `sa`/`sb`/`ss`
Swaps the **top two** nodes. With list `A -> B -> C`:
```c
if (!stack || stack->size < 2) return ;   // nothing to swap
first = stack->head;   // A
second = first->next;  // B
first->next = second->next;    // A now points to C
first->prev = second;          // A's prev becomes B (B goes on top)
if (second->next) second->next->prev = first; // C's prev becomes A
else stack->tail = first;      // only 2 elements: A becomes the tail
second->next = first;          // B points down to A
second->prev = NULL;           // B is the new top
stack->head = second;          // head is B
```
- Result: `B -> A -> C`. The guard `size < 2` matches the subject ("do nothing if
  fewer than 2"). **Correct.** `sa`/`sb` call it and print `sa`/`sb`; `ss` does both
  and prints `ss`. ⚠️ Many in-body comments (Norm).

### `ops_push.c` — `stack_pusher` + `pa`/`pb`
Moves the **top of `src`** onto the **top of `dest`**:
```c
if (!src->head) return ;            // ⚠️ dereferences src without a NULL-check on src itself
node_to_move = src->head;           // detach src's head
src->head = src->head->next;
if (src->head) src->head->prev = NULL;
else src->tail = NULL;              // src became empty
src->size--;
node_to_move->next = dest->head;    // put it on top of dest
if (dest->head) dest->head->prev = node_to_move;
else dest->tail = node_to_move;     // dest was empty → also the tail
dest->head = node_to_move;
node_to_move->prev = NULL;
dest->size++;
```
- Detaches from source top, updates source tail/size when it empties, links onto
  destination top, updates dest tail/size when it was empty. **Correct.**
- `pb` = `stack_pusher(b, a)` (A→B), prints `pb`. `pa` = `stack_pusher(a, b)` (B→A),
  prints `pa`. The arg order (`dest, src`) is right.

### `ops_rotate.c` — `stack_rotater` + `ra`/`rb`/`rr`
Rotate up: **top goes to the bottom**.
```c
if (!stack || stack->size < 2) return ;
old_head = stack->head;
new_head = old_head->next;
stack->head = new_head; new_head->prev = NULL;  // 2nd node is new top
old_head->next = NULL;
old_head->prev = stack->tail;
stack->tail->next = old_head;   // append old top after old tail
stack->tail = old_head;         // it's the new tail
```
- **Correct.** `ra`/`rb` print `ra`/`rb`; `rr` rotates both, prints `rr`.

### `ops_rev_rotate.c` — `stack_rev_rotater` + `rra`/`rrb`/`rrr`
Reverse rotate: **bottom goes to the top**.
```c
if (!stack || stack->size < 2) return ;
old_tail = stack->tail;
new_tail = old_tail->prev;
stack->tail = new_tail; new_tail->next = NULL;   // 2nd-from-bottom is new tail
old_tail->prev = NULL;
old_tail->next = stack->head;
stack->head->prev = old_tail;   // old bottom goes on top
stack->head = old_tail;
```
- **Correct** — the mirror of rotate. `rra`/`rrb`/`rrr` print accordingly (note the
  3-char writes use length 4 for the `\n`, correct).

---

## `sort_three.c` — `three_sorter`

```c
stack_o_meter(stack);                       // refresh pos + max
if (stack->max->pos == 0) ra(stack);        // max on top → send it to the bottom
else if (stack->max->pos == 1) rra(stack);  // max in the middle → pull bottom up (also lands max at bottom)
if (stack->head->value > stack->head->next->value) sa(stack); // fix the top two
```
- Strategy: **first get the max to the bottom** (position 2 of 3), then the only two
  cases left for the top two are already-ordered or one swap away → `sa` if needed.
- I traced **all 6 permutations** of `{1,2,3}` — every one ends `1,2,3` (ascending)
  with a minimal or near-minimal sequence. **Correct.**
- ⚠️ Assumes exactly 3 elements. Calling it on other sizes reads `head->next`
  blindly — only ever call it when `size == 3`.

---

## `main.c` — ⚠️ currently a **test harness, not the program**

`main(void)` (note: **no `argc/argv`**) manually:
1. mallocs `stack_a`/`stack_b` and null-inits their fields (⚠️ line 50–51 pack many
   statements on one line — a Norm violation and hard to read);
2. builds a fixed stack `24, -42, 5` with `node_creator`+`node_connecter`;
3. runs `stack_o_meter`, then demonstrates `sa`, `pb`, `ra`, `pa`, `three_sorter`,
   `is_sorted`, printing a debug table (`print_stack_debug`) after each;
4. frees both stacks.

- This is a **useful scratchpad** that proves the operations work — keep it around
  as a test (e.g. move it to a `tests/` file or behind a `#ifdef DEBUG`). But it is
  **not** a real push_swap: it never looks at the command line, so `./push_swap 5 2 8`
  does nothing with those numbers.
- `print_stack_debug` uses `printf` and ternaries — fine for a throwaway tester, but
  it must **not** ship in the final `main` (Norm forbids ternaries; the real program
  prints only operation names to stdout and `Error` to stderr).

---

## `parser.c` — written, **not wired in, and `ft_atoi_ps` is broken**

- `error_syntax(str)` — returns 1 if the token isn't a valid integer string: rejects
  empty, allows one leading `+`/`-`, requires at least one digit, rejects any
  non-digit. **Logic is correct.** (Not yet called by anything.)
- `error_duplicate(stack, dup)` — linear scan; returns 1 if `dup` already in the
  stack. Correct. (Not yet called.)
- `ft_atoi_ps(...)` — ⚠️ **three real bugs**, see `02_bugs_and_fixes.md`:
  1. after reading the sign it **doesn't skip the sign char**, so `-`/`+` is fed into
     the digit math;
  2. it applies `res = res * sign` **inside the loop every iteration** instead of once
     at the end;
  3. the overflow check `res > 2147483647 || res < -2147483648` compares an `int`
     against `INT_MAX`/`INT_MIN`, so it can **never** be true — overflow is not caught.
     Trace: `"-42"` returns `338`, not `-42`.
- `free_and_exit(...)` — frees the split array (when `is_split`), frees the stack,
  writes `"Error\n"` to **fd 2 (stderr)** — correct stream — and `exit(1)`. Good
  central error path; the only issue is `ft_atoi_ps` calls it on a broken condition.
