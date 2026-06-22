# Step 03 — Project Setup (Makefile, libft, layout, Norm)

> **Goal:** A skeleton that compiles with the required flags, links your libft,
> never relinks unnecessarily, and is organized so two people rarely touch the
> same file. Get this right once and stop thinking about it.

## What the subject requires (re-read these as a checklist)

- Language **C**, **Norm**-compliant (bonus files included in the norm check).
- Compile with **`cc -Wall -Wextra -Werror`**.
- **No unnecessary relinking** (running `make` twice with no changes must do
  nothing).
- Makefile rules: **`$(NAME) all clean fclean re`**, plus **`bonus`** for the
  checker.
- **No global variables.**
- Allowed external functions: `read, write, malloc, free, exit`, and `ft_printf`
  (or an equivalent **you** wrote). **libft is authorized.**
- If you use libft, copy its **sources + its Makefile** into a `libft/` folder, and
  your Makefile compiles libft *by calling its Makefile*, then builds the project.

## Concepts you need

### Why "no relink" matters and how it's done
Relinking means rebuilding the binary even when nothing changed. It's caused by
Makefiles that depend on `.c` files directly or use phony targets as
prerequisites of the binary. The fix is the standard pattern: the binary depends
on **`.o` object files**, and each `.o` depends on its `.c` (and your headers).
Make then rebuilds only what changed. Learn the difference between a **pattern
rule** (`%.o: %.c`) and listing objects explicitly. Make sure your header is a
prerequisite of the objects, or editing the header won't trigger rebuilds.

### Why no globals
A global variable is shared mutable state. The subject forbids it to force you to
**pass your stack/context struct around** by pointer. This is good design anyway.
Plan for a single "context" struct you thread through your functions (more in
Step 04).

### The Norm (42 norm) — the cheap points you'll lose if you ignore it
Run `norminette` constantly, not at the end. The classic killers:
- Functions longer than 25 lines, or more than 5 functions per file.
- More than 4 parameters per function.
- Forbidden `for`/`do-while`? (Some norm versions restrict; check yours.) Use
  `while`.
- Variable declarations not at the top of the block.
- No `;` on the line where a control structure opens its block, etc.

Because algorithms here can get long, **decompose early**. A 40-line sort split
into helpers is both norm-passing and more defensible.

## Suggested file layout

Many small files = fewer merge conflicts and easier norm compliance. A layout
that mirrors the build order:

```
push_swap/
├── Makefile
├── include/
│   └── push_swap.h          # the struct, prototypes, op enum
├── libft/                   # your libft + its own Makefile
├── src/
│   ├── main.c               # entry point, top-level flow
│   ├── parse.c              # argument validation + stack build
│   ├── stack_utils.c        # size, is_sorted, find min/max, etc.
│   ├── ops_swap.c           # sa, sb, ss
│   ├── ops_push.c           # pa, pb
│   ├── ops_rotate.c         # ra, rb, rr
│   ├── ops_rrotate.c        # rra, rrb, rrr
│   ├── disorder.c           # the mandatory metric
│   ├── sort_simple.c        # O(n^2)
│   ├── sort_medium.c        # O(n*sqrt(n))
│   ├── sort_complex.c       # O(n log n)
│   ├── sort_adaptive.c      # dispatch by disorder
│   ├── dispatch.c           # flag parsing -> strategy
│   └── bench.c              # --bench reporting
└── bonus/
    └── checker_bonus.c ...  # bonus program (separate build)
```
Adjust to taste, but **decide the layout with your teammate before writing**, and
write the header (`push_swap.h`) together — it's the contract between all files.

## Manual: build the skeleton

1. Write `push_swap.h` with include guards. Leave the struct empty for now (you'll
   fill it in Step 04). Add the project's standard includes.
2. Write a Makefile that:
   - Defines `NAME = push_swap`, `CC = cc`, `CFLAGS = -Wall -Wextra -Werror`.
   - Lists source files in a `SRCS` variable, derives `OBJS` from it.
   - Has a rule to build libft by invoking `make -C libft`.
   - Builds `$(NAME)` from `$(OBJS)` + the libft archive, with `-I include
     -I libft`.
   - Has `all` (→ `$(NAME)`), `clean` (rm objects), `fclean` (clean + rm binary +
     `make -C libft fclean`), `re` (fclean + all), and `bonus`.
3. Write a trivial `main` that just returns 0. Run `make`. Run `make` again — the
   second run must say "Nothing to be done." If it rebuilds, your dependencies are
   wrong (binary depends on `.c` instead of `.o`, or `.PHONY` is misused).

## Verify before moving on
- `make` then `make` again → no relink the second time. ✅
- `make re` rebuilds everything. ✅
- `make fclean` leaves no `.o` and no binary. ✅
- `norminette` is clean on what exists so far. ✅

➡ Next: `04_data_structure.md`
