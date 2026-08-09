//
// Created by nofilkhan on 7/25/26.
//

#include "CommandParser.h"
#include "Repository.h"
#include <iostream>
#include <string>

int CommandParser::run(int argc, char* argv[]) {
    std::string cmd = argv[1];

    if (cmd == "init") {
        return Repository::init();
    }

    std::cerr << "unknown command: " << cmd << "\n";
    return 1;
}
