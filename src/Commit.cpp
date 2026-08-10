//
// Created by nofilkhan on 8/10/26.
//
// Storage format of .mygit/commits/<id>.commit:
//
//   id: c3
//   parent: c2
//   timestamp: 2026-08-10 06:44:12
//   message: added notes
//   files:
//   notes.txt c3_notes.txt
//
// Everything after the "files:" line is one "<name> <object>" pair per line.

#include "Commit.h"
#include "Utils.h"
#include <sstream>

// Directory holding all commit files.
static const std::string COMMITS_DIR = ".mygit/commits";
// File holding the id of the latest commit.
static const std::string HEAD_PATH = ".mygit/HEAD";

// Serialize this commit and write it to its own file under .mygit/commits.
bool Commit::save() const {
	std::ostringstream out;
	out << "id: " << id << "\n";
	out << "parent: " << parent << "\n";
	out << "timestamp: " << timestamp << "\n";
	out << "message: " << message << "\n";
	out << "files:\n";
	for (const FileEntry& f : files) {
		out << f.name << " " << f.object << "\n";
	}
	return Utils::writeFile(COMMITS_DIR + "/" + id + ".commit", out.str());
}

// Parse a commit file back into a Commit object.
bool Commit::load(const std::string& id, Commit& out) {
	std::string path = COMMITS_DIR + "/" + id + ".commit";
	if (!Utils::pathExists(path)) return false;

	std::stringstream ss(Utils::readFile(path));
	std::string line;
	bool inFiles = false;

	while (std::getline(ss, line)) {
		if (line.empty()) continue;

		if (inFiles) {
			// "<name> <object>" - split on the last space so the file name
			// keeps everything before the snapshot name.
			size_t sep = line.rfind(' ');
			if (sep == std::string::npos) continue;
			FileEntry e;
			e.name = line.substr(0, sep);
			e.object = line.substr(sep + 1);
			out.files.push_back(e);
			continue;
		}

		if (line == "files:") { inFiles = true; continue; }

		size_t sep = line.find(": ");
		std::string key = (sep == std::string::npos) ? line : line.substr(0, sep);
		std::string value = (sep == std::string::npos) ? "" : line.substr(sep + 2);

		// A header with an empty value ("parent:") loses the trailing space,
		// so handle that shape too.
		if (sep == std::string::npos && !line.empty() && line.back() == ':') {
			key = line.substr(0, line.size() - 1);
		}

		if (key == "id") out.id = value;
		else if (key == "parent") out.parent = value;
		else if (key == "timestamp") out.timestamp = value;
		else if (key == "message") out.message = value;
	}
	return true;
}

// Return the id stored in HEAD, or "" when no commit exists yet.
std::string Commit::getHead() {
	std::string head = Utils::readFile(HEAD_PATH);
	while (!head.empty() && (head.back() == '\n' || head.back() == '\r')) head.pop_back();
	return head;
}

// Point HEAD at the given commit id.
void Commit::setHead(const std::string& id) {
	Utils::writeFile(HEAD_PATH, id);
}

// Ids are sequential ("c1", "c2", ...). The next id is one past the highest
// number already present in .mygit/commits, so committing after a checkout to
// an older commit can never reuse an existing id.
std::string Commit::nextId() {
	int highest = 0;
	for (const std::string& f : Utils::listFiles(COMMITS_DIR)) {
		// f looks like "c12.commit"
		size_t dot = f.find('.');
		if (f.size() < 2 || f[0] != 'c' || dot == std::string::npos) continue;
		try {
			int n = std::stoi(f.substr(1, dot - 1));
			if (n > highest) highest = n;
		} catch (...) {
			continue;
		}
	}
	return "c" + std::to_string(highest + 1);
}
