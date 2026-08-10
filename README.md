# MiniGit

A miniature version control system written in C++17, using only the standard
library (`std::filesystem`, `fstream`) — no external dependencies.

**Author:** Nofil Ali — **ID:** BSCS24087

Running `./minigit` with no arguments prints a landing page describing the
project and the available commands.

---

## Build

With CMake:

```bash
cmake -S . -B build
cmake --build build
# binary: build/minigit
```

Or directly with g++:

```bash
g++ -std=c++17 src/*.cpp -o minigit
```

## Quick start

```bash
mkdir test-repo && cd test-repo
../minigit init
echo "hello" > a.txt
../minigit add a.txt
../minigit commit "first commit"
../minigit log
../minigit status
../minigit checkout c1
```

## Commands

| Command | What it does |
| --- | --- |
| `minigit` | Landing page: author info, project story, usage |
| `minigit init` | Create `.mygit/` in the current folder |
| `minigit add <file>` | Stage a file for the next commit |
| `minigit commit "message"` | Snapshot all staged files as a new commit |
| `minigit log` | Print history from HEAD backwards (id, date, message) |
| `minigit status` | Show staged, modified, untracked (and deleted) files |
| `minigit checkout <commitID>` | Restore that commit's files and move HEAD |

Commit ids are sequential: `c1`, `c2`, `c3`, …

## Repository layout

```
.mygit/
├── HEAD                 # id of the latest commit ("" before the first commit)
├── index                # staging area, one filename per line
├── commits/
│   └── c1.commit        # one file per commit
└── objects/
    └── c1_a.txt         # snapshot of a.txt as of commit c1
```

### Commit file format

```
id: c2
parent: c1
timestamp: 2026-08-10 06:48:13
message: second commit: change a
files:
a.txt c2_a.txt
b.txt c1_b.txt
```

Every line after `files:` is a `<working-dir name> <object name>` pair. Files
that were tracked by the parent but not re-staged keep pointing at their older
snapshot, so each commit describes the complete state of the project.

## Data structures used

- **Singly linked list — commit history.** Each commit stores the id of its
  `parent`, and `HEAD` points at the newest node. `log` is a traversal of that
  list from HEAD until a commit with an empty parent (the first commit).
  Insertion is O(1): the new commit simply points at the old HEAD.
- **Dynamic array (`std::vector`) — staging area and file lists.** The index
  file is read into a `std::vector<std::string>` of filenames; a commit's
  tracked files are a `std::vector<FileEntry>`. Both need ordered iteration and
  cheap append, which is exactly what a dynamic array gives.

## Architecture

```
main.cpp          argc/argv guard; no args -> landing page
   |
CommandParser     maps argv[1] to a Repository call; owns usage + landing text
   |
Repository        the six commands: init, add, commit, log, status, checkout
   |         \
Index          Commit        Index = staging area (.mygit/index)
   \           /             Commit = one history node, load/save/HEAD/nextId
     Utils                   single I/O boundary: exists, mkdir, read, write,
                             copy, list, timestamp — all std::filesystem
```

Every layer only talks to the one below it, and all filesystem access goes
through `Utils`, so the rest of the code never touches `std::filesystem`
directly.

## Scope and limitations

Deliberately out of scope for this version: branches, merging, diffing, tags,
hashing/deduplicated object storage, and directory trees. `status` and `add`
work on regular files in the repository root, and filenames are assumed not to
contain spaces (the commit format splits name/object on the last space).
