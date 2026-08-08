//
// Created by nofilkhan on 7/25/26.
//

#ifndef MINIGIT_UTILS_H
#define MINIGIT_UTILS_H

#include <string>

namespace Utils {
    bool pathExists(const std::string& path);
    bool createDir(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);
    std::string readFile(const std::string& path);

};


#endif //MINIGIT_UTILS_H
