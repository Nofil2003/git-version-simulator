//
// Created by nofilkhan on 7/25/26.
//

#include "Repository.h"
#include "Utils.h"
#include "Index.h"
#include "Commit.h"
#include <iostream>
#include <vector>
#include <algorithm>

const std::string Repository::REPO_DIR = ".mygit";

int Repository::add(const std::string& filename) {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }
    // Report a re-add as "already staged" instead of claiming a second stage.
    bool wasStaged = Index::isStaged(filename);
    if (!Index::add(filename)) return 1;
    std::cout << (wasStaged ? "already staged: " : "staged: ") << filename << "\n";
    return 0;
}

int Repository::init() {
    if (Utils::pathExists(REPO_DIR)) {
        std::cerr << "repo already exists\n";
        return 1;
    }

    Utils::createDir(REPO_DIR);
    Utils::createDir(REPO_DIR + "/commits");
    Utils::createDir(REPO_DIR + "/objects");

    Utils::writeFile(REPO_DIR + "/HEAD", "");
    Utils::writeFile(REPO_DIR + "/index", "");

    std::cout << "initialized empty minigit repo in " << REPO_DIR << "\n";
    return 0;
}

// Snapshot every staged file, write a new commit node linked to the current
// HEAD, move HEAD forward and clear the staging area.
int Repository::commit(const std::string& message) {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }

    std::vector<std::string> staged = Index::getStagedFiles();
    if (staged.empty()) {
        std::cerr << "nothing staged to commit\n";
        return 1;
    }

    Commit c;
    c.id = Commit::nextId();
    c.parent = Commit::getHead();      // links this node to the previous one
    c.timestamp = Utils::timestamp();
    c.message = message;

    // Copy each staged file into .mygit/objects as <commitID>_<filename>.
    for (const std::string& name : staged) {
        if (!Utils::pathExists(name)) {
            std::cerr << "warning: staged file missing, skipped: " << name << "\n";
            continue;
        }
        FileEntry e;
        e.name = name;
        e.object = c.id + "_" + name;
        if (!Utils::copyFile(name, REPO_DIR + "/objects/" + e.object)) {
            std::cerr << "failed to snapshot: " << name << "\n";
            return 1;
        }
        c.files.push_back(e);
    }

    // Files tracked by the parent commit but not staged now must stay tracked,
    // so carry their previous snapshots forward into this commit.
    Commit parent;
    if (!c.parent.empty() && Commit::load(c.parent, parent)) {
        for (const FileEntry& old : parent.files) {
            bool alreadyIn = false;
            for (const FileEntry& e : c.files) {
                if (e.name == old.name) { alreadyIn = true; break; }
            }
            if (!alreadyIn) c.files.push_back(old);
        }
    }

    if (!c.save()) {
        std::cerr << "failed to write commit\n";
        return 1;
    }
    Commit::setHead(c.id);
    Index::clear();

    std::cout << "[" << c.id << "] " << c.message << "\n";
    std::cout << c.files.size() << " file(s) tracked\n";
    return 0;
}

// Walk the linked list of commits from HEAD back to the first commit,
// printing id, date and message for each node.
int Repository::log() {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }

    std::string cur = Commit::getHead();
    if (cur.empty()) {
        std::cout << "no commits yet\n";
        return 0;
    }

    while (!cur.empty()) {
        Commit c;
        if (!Commit::load(cur, c)) {
            std::cerr << "broken history: missing commit " << cur << "\n";
            return 1;
        }
        std::cout << "commit " << c.id << "\n";
        std::cout << "Date:    " << c.timestamp << "\n";
        std::cout << "Message: " << c.message << "\n\n";
        cur = c.parent;     // step to the parent node
    }
    return 0;
}

// Compare the working directory against the last commit and the staging area.
// Reports staged files, modified tracked files and untracked files.
int Repository::status() {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }

    std::vector<std::string> staged = Index::getStagedFiles();

    // Load the snapshot list of the current HEAD commit, if there is one.
    Commit head;
    std::string headId = Commit::getHead();
    bool hasHead = !headId.empty() && Commit::load(headId, head);

    std::vector<std::string> modified;
    std::vector<std::string> untracked;
    std::vector<std::string> deleted;

    std::vector<std::string> working = Utils::listFiles(".");

    for (const std::string& name : working) {
        // Find this file in the last commit.
        const FileEntry* tracked = nullptr;
        if (hasHead) {
            for (const FileEntry& e : head.files) {
                if (e.name == name) { tracked = &e; break; }
            }
        }

        bool isStaged = std::find(staged.begin(), staged.end(), name) != staged.end();

        if (!tracked) {
            if (!isStaged) untracked.push_back(name);
            continue;
        }

        // Tracked: compare the working copy against its stored snapshot.
        std::string snapshot = Utils::readFile(REPO_DIR + "/objects/" + tracked->object);
        if (Utils::readFile(name) != snapshot && !isStaged) {
            modified.push_back(name);
        }
    }

    // Tracked files that no longer exist in the working directory.
    if (hasHead) {
        for (const FileEntry& e : head.files) {
            if (!Utils::pathExists(e.name)) deleted.push_back(e.name);
        }
    }

    std::cout << "on commit: " << (headId.empty() ? "(none)" : headId) << "\n\n";

    std::cout << "staged files:\n";
    if (staged.empty()) std::cout << "  (none)\n";
    for (const std::string& f : staged) std::cout << "  " << f << "\n";

    std::cout << "\nmodified files:\n";
    if (modified.empty()) std::cout << "  (none)\n";
    for (const std::string& f : modified) std::cout << "  " << f << "\n";

    std::cout << "\nuntracked files:\n";
    if (untracked.empty()) std::cout << "  (none)\n";
    for (const std::string& f : untracked) std::cout << "  " << f << "\n";

    if (!deleted.empty()) {
        std::cout << "\ndeleted files:\n";
        for (const std::string& f : deleted) std::cout << "  " << f << "\n";
    }
    return 0;
}

// Restore every file snapshot of the given commit into the working directory
// and move HEAD to that commit.
int Repository::checkout(const std::string& commitId) {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }

    Commit c;
    if (!Commit::load(commitId, c)) {
        std::cerr << "no such commit: " << commitId << "\n";
        return 1;
    }

    for (const FileEntry& e : c.files) {
        std::string object = REPO_DIR + "/objects/" + e.object;
        if (!Utils::pathExists(object)) {
            std::cerr << "warning: missing snapshot for " << e.name << "\n";
            continue;
        }
        if (!Utils::copyFile(object, e.name)) {
            std::cerr << "failed to restore: " << e.name << "\n";
            return 1;
        }
        std::cout << "restored: " << e.name << "\n";
    }

    Commit::setHead(c.id);
    Index::clear();     // staged edits do not survive a checkout
    std::cout << "HEAD is now at " << c.id << " (" << c.message << ")\n";
    return 0;
}
