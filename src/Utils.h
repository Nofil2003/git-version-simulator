//
// Created by nofilkhan on 7/25/26.
//

#ifndef MINIGIT_UTILS_H
#define MINIGIT_UTILS_H

#include <string>
#include <vector>

namespace Utils {
    bool pathExists(const std::string& path);
    bool createDir(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);
    std::string readFile(const std::string& path);

    // Copy a file, overwriting the destination if it is already there.
    bool copyFile(const std::string& from, const std::string& to);
    // List regular files in a directory (non-recursive), skipping .mygit.
    std::vector<std::string> listFiles(const std::string& dir);
    // Human readable current time, e.g. "2026-08-10 06:44:12".
    std::string timestamp();
};


#endif //MINIGIT_UTILS_H
