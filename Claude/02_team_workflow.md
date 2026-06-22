# Step 02 — Team Workflow (2 learners)

> **Goal:** Split the work so both of you contribute meaningfully, both understand
> *everything*, and you don't spend the project fixing merge conflicts. The subject
> is explicit: **both** of you must be able to explain **any** part at defense, and
> both logins must appear in the repo.

## The trap to avoid

The natural instinct is "you do parsing, I do algorithms." If you do that purely,
one of you can't defend half the project and you both fail. The rule: **split the
*writing*, share the *understanding*.** Pair-program the hard parts (the three
algorithms). Divide the mechanical parts.

## A workable division of labour

Think in two tracks that can progress in parallel after the foundation is shared:

**Shared foundation (build together, same session):**
- Step 04 data structure — you must both agree on it or nothing fits together.
- Step 06 operations — small, and everything depends on them being correct.

**Track A (e.g. learner 1):**
- Parsing & error handling (Step 05)
- CLI / dispatch / flag parsing (Step 12)
- Benchmark mode (Step 13)
- Bonus checker (Step 15)

**Track B (e.g. learner 2):**
- Disorder metric (Step 07)
- Simple O(n²) strategy (Step 08)
- Medium O(n√n) strategy (Step 09)

**Pair-program together (too important to silo):**
- Complex O(n log n) strategy (Step 10) — this is what makes or breaks the benchmark.
- Adaptive dispatch (Step 11) — it stitches everyone's work together.
- Optimization pass (Step 14).

Then **swap and review**: each person reads and explains the other's code back to
them. If you can't explain it, it's not done.

## Git hygiene for two people

The subject says only the Git repo is graded and both must show as contributors.

- Each person commits **from their own machine / account** so authorship is real.
  Don't pair on one keyboard for everything, or only one login shows up.
- Agree on a branch convention. Simplest that works: short-lived branches per
  feature (`parse`, `radix`, `bench`) merged into `main` often. Long-lived
  divergent branches = pain.
- Pull before you start, merge small and often. The longer two branches drift, the
  worse the conflict.
- Decide **file ownership** up front (see Step 03 file layout) so you rarely edit
  the same file simultaneously. Conflicts mostly come from two people editing one
  big file — many small files prevent that.

> Note: this working directory is **not** currently a git repo. When you create
> the real submission repo, `git init` it (or clone the assigned one) and commit
> early. Keep this `Claude/` folder out of the graded repo if it isn't meant to be
> submitted — only your sources, Makefile, README, and (optional) test files belong.

## Definition of "done" for any task

Before you mark something done and hand it to your teammate:
1. It compiles clean with `-Wall -Wextra -Werror`.
2. It passes the Norm.
3. It has no leaks (`valgrind` clean) and no crashes on edge cases.
4. You can explain *why* it works, not just *that* it works.

➡ Next: `03_project_setup.md`
