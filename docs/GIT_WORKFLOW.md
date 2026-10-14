# Git Workflow

## 1. Start from an up-to-date `master`

Before creating a new branch, make sure your local `master` has everything that's already been merged:

```bash
git checkout master
git pull origin master
```

## 2. Create your branch

One branch per feature/fix, named so others immediately know what it's for:

```bash
git checkout -b feat/ft_split
```

Naming convention: `feat/...` for new functionality, `fix/...` for bug fixes, `chore/...` / `docs/...` for non-code changes.

## 3. Work and commit

Commit as you go, with clear messages:

```bash
git add ft_split.c
git commit -m "feat: implement ft_split"
```

Small, focused commits are easier for reviewers (and future you) to follow than one giant commit at the end.

## 4. Before opening a PR: sync with `master` again

Other PRs may have merged into `master` while you were working. Check:

```bash
git fetch origin
git log HEAD..origin/master --oneline
```

- **No output** → nothing new, you're already current, skip to step 5.
- **Shows commits** → bring them in before opening the PR:

```bash
git merge origin/master
```

If this produces conflicts, resolve them now, on your machine, while it's just you dealing with it. Not later, in front of a reviewer on GitHub.

```bash
# after fixing conflicting files by hand:
git add <file>
git commit
```

Then push:

```bash
git push origin feat/ft_split
```

(First push of a new branch: `git push -u origin feat/ft_split`)

## 5. Open the PR on GitHub

1. Go to the repo on GitHub -> **"Compare & pull request"**.
2. Check **base: master** ← **compare: your-branch**.
3. Write a clear **title** (not the raw branch name).
4. Write a **description**: what this PR does, what it deliberately leaves out, anything a reviewer should pay special attention to.
5. Add your teammates as **reviewers** (sidebar).
6. Click **Create pull request**.

## 6. Review

Teammates read the diff, leave comments, request changes or approve.

If changes are requested:

```bash
# make the fixes on the SAME branch
git add <file>
git commit -m "address review: ..."
git push origin feat/ft_split
```

This just adds commits to the same PR, no need to open a new one.

## 7. Before merging: make sure it's still conflict-free

If the PR sat open for a while, `master` may have moved again (another PR merged in the meantime). GitHub will tell you directly on the PR page:

- **"Able to merge" (green)** → good to go.
- **"This branch has conflicts that must be resolved"** → pull `master` into your branch again, same as step 4, resolve, push. The PR updates automatically.

**Resolve conflicts before merging, never after.**  and it's much easier to resolve the conflicts locally and not on GitHub's web editor.

## 8. Merge

Once approved and conflict-free, whoever has merge rights clicks the green **Merge pull request** button on GitHub.

Use **Squash and merge** (pick it from the dropdown next to the button): it flattens all of your branch's commits into a single, clean commit on `master`, instead of dragging in every "wip"/"fix typo" commit. This keeps `master`'s history readable. One line per feature/fix.

> On `Squash and merge` every commit on `master` is a reviewed, complete PR. If something breaks, you go back PR-by-PR (`git log --oneline master`, `git revert <commit>`, `git bisect`) instead of digging through each commit, which usually weren't working states anyway. Nothing is deleted either. GitHub still keeps a PR's original, unsquashed commits under its **Commits** tab even after merge, if you ever need to look.

After merging, GitHub offers to **delete the branch**, do it. It's merged, it has no further use, and it keeps the branch list from piling up.

## 9. Clean up locally

```bash
git checkout master
git pull origin master
git branch -d feat/ft_split
```

`-d` only deletes it if it's fully merged, git itself protects you from losing unmerged work here.

---

## Quick reference

```
master ──────●───────●───────●──────●───►   (always deployable, PR-reviewed only)
              \             /
               feat/x ──●──●   (your branch: commit, push, PR, squash-merge, delete)
```

