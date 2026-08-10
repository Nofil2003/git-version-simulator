#!/usr/bin/env bash
# Demo of all six MiniGit commands, run end to end in this folder.
# Usage:  cd test-repo && ./demo.sh
# Starts from a clean slate every time (wipes .mygit and the demo files).

set -e

MINIGIT="../minigit"

# Print a section header so the output is easy to follow.
step() {
    echo
    echo "=============================================================="
    echo ">> $*"
    echo "=============================================================="
}

# Show the command, then run it.
run() {
    echo "\$ minigit $*"
    "$MINIGIT" "$@"
}

cd "$(dirname "$0")"

if [ ! -x "$MINIGIT" ]; then
    echo "minigit binary not found at $MINIGIT"
    echo "build it first:  g++ -std=c++17 src/*.cpp -o minigit"
    exit 1
fi

# Clean slate: remove the repo and any files from a previous demo run.
rm -rf .mygit a.txt b.txt scratch.txt

step "0. landing page (no arguments)"
"$MINIGIT" | head -8
echo "... (full page shown when you run ./minigit with no args)"

step "1. init - create the repository"
run init
echo "--- .mygit contents:"
ls -a .mygit

step "2. add - stage files"
echo "hello world, version 1" > a.txt
echo "some notes, version 1"  > b.txt
echo "temporary scratch file" > scratch.txt
run add a.txt
run add b.txt
run add a.txt          # already staged, handled gracefully

step "3. status - before the first commit"
# a.txt and b.txt are staged; scratch.txt was never added, so it is untracked.
run status

step "4. commit - snapshot the staged files"
run commit "first commit: add a.txt and b.txt"
echo "--- snapshots stored in .mygit/objects:"
ls .mygit/objects
echo "--- commit file:"
cat .mygit/commits/c1.commit

step "5. status - after editing a tracked file"
echo "hello world, version 2 (CHANGED)" > a.txt
# a.txt is now modified relative to commit c1, and still nothing is staged.
run status

step "6. second commit - only a.txt changed"
run add a.txt
run commit "second commit: change a.txt"
# b.txt was not re-staged, so c2 carries c1's snapshot of it forward.
echo "--- commit file:"
cat .mygit/commits/c2.commit

step "7. log - walk the linked list of commits from HEAD"
run log

step "8. checkout - go back to the first commit"
echo "--- a.txt before checkout:"
cat a.txt
run checkout c1
echo "--- a.txt after checkout:"
cat a.txt
echo "--- HEAD now points at: $(cat .mygit/HEAD)"

step "demo complete - all six commands exercised"
echo "init, add, commit, log, status, checkout"
