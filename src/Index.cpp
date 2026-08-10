//Created on 9/8/26 on Nofil Khan
//
// Staging area. Backed by the plain text file .mygit/index, one filename per
// line. In memory it is handled as a dynamic array (std::vector<std::string>).

#include "Index.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// Path of the index file inside the repo.
static const std::string INDEX_PATH = ".mygit/index";

// Stage one file: verify it exists on disk, skip if already staged,
// otherwise append its name to the index file.
bool Index::add(const std::string& filename){

	if (!Utils::pathExists(filename)){
		std::cerr << "file not found: " << filename << "\n";
		return false;
	}
	std::string current = Utils::readFile(INDEX_PATH);

	if (Index::isStaged(filename)) return true;

	current += filename + "\n";

	Utils::writeFile(INDEX_PATH, current);
	return true;
}

// True when the file is already listed in the staging area.
bool Index::isStaged(const std::string& filename){
	for (const std::string& f : Index::getStagedFiles()) {
		if (f == filename) return true;
	}
	return false;
}

// Read the index file and return its lines as a dynamic array of filenames.
// Blank lines are ignored so a trailing newline does not create an empty entry.
std::vector<std::string> Index::getStagedFiles(){
	std::vector<std::string> files;
	std::string content = Utils::readFile(INDEX_PATH);
	std::stringstream ss(content);
	std::string line;
	while (std::getline(ss, line)) {
		if (!line.empty()) files.push_back(line);
	}
	return files;
}

// Empty the staging area. Called after a successful commit.
void Index::clear(){
	Utils::writeFile(INDEX_PATH, "");
}
