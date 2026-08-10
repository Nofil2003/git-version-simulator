//
// Created by nofilkhan on 7/25/26.
//

#include "Repository.h"
#include "Utils.h"
#include "Index.h"
#include <iostream>

const std::string Repository::REPO_DIR = ".mygit";

int Repository::add(const std::string& filename) {
    if (!Utils::pathExists(REPO_DIR)) {
        std::cerr << "not a minigit repo\n";
        return 1;
    }
    if (!Index::add(filename)) return 1;
    std::cout << "staged: " << filename << "\n";
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
