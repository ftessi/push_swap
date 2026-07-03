# Step 05b — Readiness Review *before* you write the parser

> Written after inspecting the actual repo state (`repo/Code/`). Read this **before**
> `06_operations.md`. It answers one question: *"Is the current setup enough for me
> to start parsing & error handling?"*
>
> **Short answer: not yet.** The project doesn't compile, the data structure has a
> couple of bugs, and a few things parsing depends on are missing. None of it is
> big — but fix it first or you'll be debugging the scaffold instead of your parser.

---

## A. What's already good (keep it)

- You chose a **doubly linked list node** (`t_stack_node` with `next`/`prev`) — a
  solid, defensible choice (see `04_data_structure.md`).
- The node already carries the fields your *later* algorithms will need: `index`
  (rank), `pos`, `median`, `cost`, `target`. Good forward thinking — you won't
  touch most of these during parsing, but they're correctly anticipated.
- `stack_size` and `find_last` are correct and exactly what an append-to-tail
  parser needs.
- The Makefile has all required rules (`all/clean/fclean/re/bonus`) and the right
  flags.

## B. Blockers — the project does not build right now

Fix these *before* writing any parsing code, or you can't even test it.

1. **Makefile vs. real file locations.** The Makefile says `SRC_DIR = src` and
   builds `obj/main.o`, but your sources live flat in `Code/`. Running `make`
   fails: *"No rule to make target `obj/main.o'"*.
   → Decide one layout and make the Makefile match it. Either move sources into
   `Code/src/` (recommended — matches the file you already wrote and the layout in
   `03_project_setup.md`) **or** change `SRC_DIR`. Also add every new `.c` you
   create to `SRC_FILES`, or it won't be compiled or linked.

2. **`stack_utils.c` has no `#include "push_swap.h"`.** Compiled alone it reports
   *"unknown type name 't_stack_node'"* and *"undeclared identifier 'NULL'"*. Every
   `.c` must include the header.

3. **`find_max` is unfinished.** It declares variables and only handles the
   `!stack` case — there is no loop and no `return` on the normal path. Under
   `-Werror` this is a compile error ("control reaches end of non-void function").
   → Either finish it or remove it until you need it (you don't need it for
   parsing).

4. **`main()` takes no arguments.** Parsing reads the command line, so you need
   `int main(int argc, char **argv)`. Right now `main()` can't see the input at
   all.

## C. Data-structure issues to settle *now* (parsing depends on them)

You currently have **two** structs and an unresolved question: *what, exactly, is
"stack a"?*

1. **Pick your "stack handle" and commit to it.** Two consistent options:
   - **(a) Bare head pointer:** a stack *is* a `t_stack_node *` (pointer to the
     top node). Simple; `stack_size`/`find_last` already work this way.
   - **(b) Container struct:** a small struct holding `t_stack_node *head`,
     `t_stack_node *tail`, and `int size`. More bookkeeping but O(1) size/tail.

   You started option (b) with `t_stack_check` — but its `head`/`tail` are typed
   `struct s_stack_check *`. That's a **bug**: a container's head should point to a
   **node**, i.e. `t_stack_node *head;`. As written, your container points at other
   containers. Decide (a) or (b), and if (b), fix the member types.

2. **`disorder` is in the wrong places / wrong type.** Disorder is a property of a
   *whole stack measured once* (a `double` in `[0,1]`), not of every node. Having
   `double disorder` inside `t_stack_node` and `int disorder` inside the container
   is two mistakes: it shouldn't be per-node, and `int` can't hold `0.0–1.0`.
   → Remove it from the node; store it once as a `double` where you keep
   whole-stack/run state. (See note D below.)

3. **You'll soon want a context/run struct** (`04_data_structure.md` calls it
   `t_ctx`): one place holding stack a, stack b, the chosen strategy, the `--bench`
   flag, and the `op_count[11]` table. You don't strictly need it to *parse*, but
   parsing's end product (the built stack a + a clean error/free path) plugs into
   it. Sketch it now so your parser builds into the right home instead of being
   rewired later.

## D. Things parsing needs that don't exist yet

1. **Prototypes in the header.** `push_swap.h` declares structs but no functions.
   Add a prototypes section (e.g. `int stack_size(...)`, your future `parse_args`,
   node-builder, error handler). Without prototypes, other files can't call your
   parser cleanly and `-Wall` will complain.

2. **A strict string→int converter.** There is no libft folder and no
   `ft_atoi`-style helper. Plain `atoi` is **forbidden by the spirit of the
   subject** (it can't detect overflow or junk). You must write your own (covered
   conceptually in `05_parsing_and_errors.md`). Decide: bring in a `libft/` (and
   wire its Makefile per the common instructions) or write the few helpers you need
   directly. For parsing you minimally need: strict number validation + safe
   conversion, and — *if* you support the quoted-string form `"4 67 3"` — a split.

3. **Norm compliance (you'll lose points here as-is).** The current files mix
   **spaces and tabs** for indentation; the 42 Norm requires **tabs**. Run
   `norminette` now and fix indentation before you add more code, so you're not
   reformatting a large parser later. Also: the `//` comments inside the struct are
   fine (they're outside functions), but **no comments inside function bodies** —
   keep that rule in mind as you write parsing helpers.

---

## E. Your "definition of ready" checklist

Tick all of these and you're clear to write the parser:

- [ ] `make` builds with no errors/warnings, and `make` a second time does nothing
      (no relink).
- [ ] Every `.c` includes `push_swap.h`; header has a prototypes section.
- [ ] `main(int argc, char **argv)`.
- [ ] One decided stack representation (bare head **or** fixed container with
      `t_stack_node *head/tail`), used consistently.
- [ ] `disorder` removed from the node; stored once as a `double`.
- [ ] `find_max` finished or removed (don't ship a non-compiling stub).
- [ ] `norminette` clean (tabs, function length, etc.).
- [ ] You can build the stack from hardcoded values and free it with **zero leaks**
      (test before parsing real input).

---

## F. A closer path for the parser itself (no full solution)

Once the checklist is green, structure your work as small, individually testable
functions. Below are the *shapes* and the *questions each must answer* — you write
the bodies.

### F.1 Decide your argument model first
The subject's examples show both `./push_swap 4 67 3 87 23` (separate args) and
`ARG="4 67..."; ./push_swap $ARG` (shell-split, still separate args). A single
quoted `"4 67 3"` arriving as **one** `argv` entry is the case you *may* also
support by splitting on spaces. Write down which you support and be consistent.

### F.2 Functions to define (signatures, not bodies)
Think about what each returns and how it signals failure:

```
/* Is this token a well-formed integer string? sign? digits only? */
bool    is_valid_int(const char *s);

/* Convert with overflow detection. Return success/failure, write result out. */
bool    safe_atoi(const char *s, int *out);   /* or return long and check bounds */

/* Build one node from a value (malloc, init fields, links NULL). */
t_stack_node *new_node(int value);

/* Append a node to the bottom of the stack (use find_last / tail). */
void    append_node(/* your stack handle */, t_stack_node *node);

/* True if value already present in the stack. */
bool    has_duplicate(/* stack */, int value);

/* Free the whole stack; safe on NULL. */
void    free_stack(/* stack */);

/* Central failure path: free everything, write "Error\n" to fd 2, exit. */
void    error_exit(/* stack */);

/* Top-level: validate argv, build & return stack a (or error_exit). */
/* stack handle */ parse_args(int argc, char **argv);
```

### F.3 The flow, in words (derive the code yourself)
1. If `argc < 2`: nothing to sort → print nothing, `exit(0)`. (Not an error.)
2. Walk the arguments left→right. For each token:
   - (optional) if it's a flag like `--simple`, record it and skip — *but* note
     full flag handling is `12_cli_and_dispatch.md`'s job; for now decide whether
     you parse flags here or reject everything non-numeric. Pick one, document it.
   - validate it's an integer (`is_valid_int`), else `error_exit`.
   - convert it safely (`safe_atoi`), else `error_exit` (overflow).
   - check it's not a duplicate (`has_duplicate`), else `error_exit`.
   - make a node and append it (first token = **top** of the stack).
3. Return the built stack.

### F.4 Edge cases that *will* be tested (turn each into a manual test)
| Input | Expected |
|-------|----------|
| `./push_swap` | nothing, exit 0 |
| `./push_swap 42` | nothing, exit 0 (already "sorted") |
| `./push_swap 2 1` | (valid; later sorted by `sa`) |
| `./push_swap 0 one 2` | `Error\n` to **stderr** |
| `./push_swap 1 2 2` | `Error\n` (duplicate) |
| `./push_swap 2147483648` | `Error\n` (overflow) |
| `./push_swap ""` / `+` / `-` | `Error\n` (no digits) |
| `./push_swap -- 1 2` | your documented choice (error or ignore) |

### F.5 Two traps that fail real evaluations
- **Leak on the error path.** Evaluators run valgrind on *bad* input too. Before
  `exit`, your `error_exit` must `free_stack` everything already built. Test:
  `valgrind ./push_swap 1 2 two` → 0 leaks.
- **`Error` to the wrong stream.** It goes to **stderr (fd 2)**, never stdout.
  Verify: `./push_swap 1 a 2 2>/dev/null` prints **nothing** on stdout.

---

## G. Suggested order for *your* next hour
1. Fix the build (B1–B4) + struct decisions (C1–C2). Commit: "fix build + struct".
2. Add prototypes + decide on libft/helpers (D1–D2). Commit.
3. Fix indentation/norm (D3). Commit.
4. Build stack from hardcoded values, free it, valgrind clean (E last box).
5. *Then* write the parser following F. Test every row of F.4 + the F.5 traps.

When all of F.4 behaves and valgrind is clean, you're done — hand off to
`06_operations.md` (or your teammate, if they own operations).
