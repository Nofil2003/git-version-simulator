//
// Created by nofilkhan on 7/25/26.
//
// Turns argv into a call on Repository. Also owns the landing page and the
// usage text shown when minigit is run without a command.

#include "CommandParser.h"
#include "Repository.h"
#include <iostream>
#include <string>

// Print the list of supported commands.
void CommandParser::printUsage() {
    std::cout <<
        "usage: minigit <command> [args...]\n\n"
        "commands:\n"
        "  init                 create a new minigit repo in the current folder\n"
        "  add <file>           stage a file for the next commit\n"
        "  commit \"message\"     snapshot all staged files as a new commit\n"
        "  log                  show commit history, newest first\n"
        "  status               show staged, modified and untracked files\n"
        "  checkout <commitID>  restore the files of a commit and move HEAD\n";
}

// Landing page shown when minigit is run with no arguments: who built it,
// what it is, and how it was built.
void CommandParser::printLanding() {
    std::cout <<
        "MiniGit - a small version control system\n"
        "=======================================\n\n"
        "Name: Nofil Ali\n"
        "ID:   BSCS24087\n\n"
        "About this project\n"
        "------------------\n"
        "MiniGit started as an attempt to understand what actually happens when\n"
        "you type 'git commit'. Rather than guess, I rebuilt the core of it: a\n"
        "repository folder, a staging area, content snapshots, and a chain of\n"
        "commits. The commit history is a singly linked list - every commit\n"
        "stores the id of its parent, and HEAD points at the newest node, so\n"
        "'log' is simply a traversal of that list. The staging area is a dynamic\n"
        "array of filenames, loaded from and written back to a plain index file.\n"
        "Building it taught me how much of version control is careful file\n"
        "layout and disciplined state handling rather than clever algorithms,\n"
        "and it made std::filesystem and C++ file I/O far less intimidating.\n"
        "Honestly, time was the binding constraint: with a deadline in front of\n"
        "me I chose depth over breadth and kept the scope to six core commands\n"
        "that work end to end. Branching, merging, diffing, tags and hashed\n"
        "object storage are deliberately left out - they were the next steps,\n"
        "not forgotten ones.\n\n";
    printUsage();
}

// Dispatch: argv[1] is the command, the rest are its arguments.
int CommandParser::run(int argc, char* argv[]) {
    std::string cmd = argv[1];

    if (cmd == "init") {
        return Repository::init();
    }

    if (cmd == "add") {
        if (argc < 3) {
            std::cerr << "usage: minigit add <file>\n";
            return 1;
        }
        return Repository::add(argv[2]);
    }

    if (cmd == "commit") {
        if (argc < 3) {
            std::cerr << "usage: minigit commit \"message\"\n";
            return 1;
        }
        return Repository::commit(argv[2]);
    }

    if (cmd == "log") {
        return Repository::log();
    }

    if (cmd == "status") {
        return Repository::status();
    }

    if (cmd == "checkout") {
        if (argc < 3) {
            std::cerr << "usage: minigit checkout <commitID>\n";
            return 1;
        }
        return Repository::checkout(argv[2]);
    }

    if (cmd == "help" || cmd == "--help" || cmd == "-h") {
        printUsage();
        return 0;
    }

    std::cerr << "unknown command: " << cmd << "\n";
    printUsage();
    return 1;
}
