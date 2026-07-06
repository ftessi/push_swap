# Git recovery — 2026-07-06

> You asked me to restore files that a teammate's commit "reversed" back to your
> commit `248cc43`, on local + remote, without touching their new files — and to save
> every command with an explanation. **I investigated first, and the situation turned
> out to be different from what it looked like.** This file documents what actually
> happened, the safe fix I applied, and why a force-push would have been the *wrong*
> and destructive tool.

---

## TL;DR

- **Nothing of yours was reversed.** Your teammate's commit `6e86783 "simple algorithm"`
  is purely **additive** — it added `simple.c`, `algorithm_utils.c`, `psodo.txt`, plus
  a one-line `simple_sorter` prototype in `push_swap.h` and a comment-typo fix in
  `sort_three.c`. Your `build_stack.c` and `tests/bench_main.c` were never touched.
- The pull that brought their commit in was a **fast-forward** (reflog proves it) —
  **no force-push, no history rewrite.**
- What actually "disappeared" was your *uncommitted* walkthrough work
  (`main(argc,argv)`, the `long ft_atoi_ps` overflow fix, header + Makefile edits).
  **It was never committed** — commit `248cc43` only added two new files. Your edits
  were sitting safely in **`stash@{1}`**.
- **Fix applied:** recovered your work from the stash and merged it on top of your
  teammate's commit (resolving one trivial `push_swap.h` conflict by keeping *both*
  your changes and their `simple_sorter` prototype). Committed locally as `18b94a7`.
- **A force-restore to `248cc43` would have deleted your teammate's `simple.c` /
  `algorithm_utils.c`** — the opposite of what you wanted. We did not do that.

---

## Part 1 — Diagnosis (all read-only, safe to re-run)

```sh
# Where are we, and what's the shape of history?
git log --oneline --graph -20
git status
git rev-parse HEAD

# Line up local main, remote main, and your commit.
git fetch --all
git log --oneline -10 origin/main
git merge-base --is-ancestor 248cc43 origin/main && echo "248cc43 is an ancestor of origin/main"
```
This showed the history is **linear**: `248cc43` (yours) → `6e86783` (teammate) with
`6e86783` sitting on top of `248cc43` on both local and remote `main`.

```sh
# What did the teammate's commit actually change vs. your commit?
git diff --name-status 248cc43 6e86783
#   A  Code/algorithm_utils.c     <- new file (keep)
#   M  Code/push_swap.h           <- +1 prototype (additive)
#   A  Code/simple.c              <- new file (keep)
#   M  Code/sort_three.c          <- comment typo fix only
#   A  psodo.txt                  <- new file (keep)

# What did YOUR commit 248cc43 actually change vs. its parent?
git diff --name-status f063ba5 248cc43
#   A  Code/build_stack.c         <- your new file
#   A  Code/tests/bench_main.c    <- your new file
# (i.e. 248cc43 added files; it did NOT modify main.c/parser.c/push_swap.h)

# Are your added files still present at the teammate's HEAD? (yes)
git ls-tree 6e86783 Code/build_stack.c Code/tests/bench_main.c
```

Key realization: at `248cc43`, `main.c` was still the old test harness and `parser.c`
still had the broken `int ft_atoi_ps`. So the "real main + parser fix" were **not in
any commit**. Where were they?

```sh
# Was history ever force-pushed / rewritten? (No — every pull was a fast-forward.)
git reflog | grep -iE "pull|force|rebase"

# Any lost work in stashes or dangling objects?
git stash list
#   stash@{1}: WIP on main: 248cc43 ...   <-- this one
git fsck --no-reflogs --lost-found | grep -E "dangling (commit|blob)"

# Confirm the stash holds exactly your walkthrough implementation:
git stash show --name-only 'stash@{1}'          # main.c, parser.c, push_swap.h, Makefile
git show 'stash@{1}:Code/main.c'   | grep -n "main(int argc"        # real main  ✓
git show 'stash@{1}:Code/parser.c' | grep -n "long\|res \* sign"    # atoi fix   ✓
```
**Found it.** Your work was in `stash@{1}`, based on `248cc43`.

**Why it happened (most likely):** you implemented the walkthrough in your working
tree but committed only the two *new* files (`git add` missed the *modified* ones).
When you later pulled your teammate's commit, git stashed your uncommitted edits (or
you stashed them yourself) — so they vanished from the working tree but were never
lost. This is the "reversed my files" feeling, but git didn't reverse anything.

---

## Part 2 — The safe recovery (what I ran)

The correct operation is **not** `reset`/force-push. It's: replay your stashed work on
top of the teammate's commit and merge. Non-destructive, no force.

```sh
# 1. Scratch branch off the teammate's HEAD so nothing shared is at risk yet.
git checkout -b recover-scratch main            # main == 6e86783

# 2. Re-apply your stashed work. Use 'apply' (NOT 'pop') so the stash survives if
#    anything goes wrong.
git stash apply 'stash@{1}'
#    -> main.c, parser.c, Makefile apply cleanly
#    -> CONFLICT in push_swap.h (you AND the teammate both edited it)

# 3. Resolve push_swap.h by KEEPING BOTH sides:
#      - your changes: 'long ft_atoi_ps', build_stack prototype, aligned formatting
#      - their addition: 'void simple_sorter(t_stack_check *a, t_stack_check *b);'
#    (Two conflict hunks: the ALGORITHMS section, and a 42-header date line.)
git add Code/push_swap.h

# 4. Prove it builds and your recovered program works.
make -C Code re
./Code/push_swap 3 1 2          # -> ra
./Code/push_swap 2147483648     # -> Error   (your overflow fix)
./Code/push_swap 1 2 2          # -> Error   (duplicate)
make -C Code fclean             # don't commit build artifacts

# 5. Commit the recovery, then fast-forward main onto it and delete the scratch.
git commit -m "Restore ftessi parser/main/Makefile work and merge with teammate simple sort"
git checkout main
git merge --ff-only recover-scratch
git branch -d recover-scratch
```

Result — local `main` is now:
```
18b94a7  Restore ftessi parser/main/Makefile work and merge with teammate simple sort
6e86783  simple algorithm            (teammate — untouched)
248cc43  Parser and build_stack ...  (you)
```
Teammate's `simple.c`, `algorithm_utils.c`, `psodo.txt` are all still present.

### Note on two side effects (both fine / intentional)
- The commit also **removes the tracked `push_swap` binary** (a build artifact that
  shouldn't be in git — your stash already had it deleted). Good hygiene.
- `simple.c` / `algorithm_utils.c` exist but are **not yet in the Makefile `SRC`**, so
  the teammate's `simple_sorter` isn't compiled/linked yet. Nothing calls it, so it
  builds clean. Wiring it into `sort_dispatch` is a *next integration step*, not part
  of this recovery.

---

## Part 3 — Publishing to the remote (the one step left)

Because your local `main` is just `origin/main` **plus one new commit**, this is a
normal **fast-forward push — no `--force` needed, and it destroys nothing**:

```sh
git push origin main
```

> ⚠️ **Do NOT** use `git push --force` or `git reset --hard 248cc43` here. That would
> rewrite shared history and delete your teammate's `simple.c` / `algorithm_utils.c`.
> The whole point of this recovery was to avoid that.

**Courtesy:** tell your teammate you're pushing a commit on top of theirs so they
`git pull` before their next change.

---

## Part 4 — Cleanup once you've confirmed everything is good

```sh
git stash drop 'stash@{1}'          # only after you've verified the recovery
git branch -d give-me-back-my-work  # the temporary branch at 248cc43, no longer needed
# stash@{0} (WIP on 6e86783: push_swap.h) and stash@{2} are older/leftover — inspect
# with 'git stash show -p stash@{0}' before dropping.
```

I left `stash@{1}` **in place** as a safety net — drop it only when you're satisfied.
