//
// Created by nofilkhan on 8/10/26.
//
// One node of the commit history. The history itself is a singly linked list:
// every commit stores the id of its parent, and HEAD points at the newest node.

#ifndef MINIGIT_COMMIT_H
#define MINIGIT_COMMIT_H

#include <string>
#include <vector>

// A single tracked file inside a commit: its path in the working directory and
// the name of its saved snapshot inside .mygit/objects.
struct FileEntry {
	std::string name;
	std::string object;
};

class Commit {
public:
	std::string id;
	std::string parent;      // empty for the very first commit
	std::string timestamp;
	std::string message;
	std::vector<FileEntry> files;   // dynamic array of tracked files

	// Write this commit to .mygit/commits/<id>.commit.
	bool save() const;

	// Load a commit by id. Returns false if the commit file does not exist.
	static bool load(const std::string& id, Commit& out);

	// Read/write .mygit/HEAD, which holds the id of the latest commit.
	static std::string getHead();
	static void setHead(const std::string& id);

	// Generate the id for the next commit ("c1", "c2", ...).
	static std::string nextId();
};

#endif //MINIGIT_COMMIT_H
