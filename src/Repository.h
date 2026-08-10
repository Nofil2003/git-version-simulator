//
// Created by nofilkhan on 7/25/26.
//

#ifndef MINIGIT_REPOSITORY_H
#define MINIGIT_REPOSITORY_H
#include <string>

class Repository {
public:
	static const std::string REPO_DIR;
	static int init();
	static int add(const std::string& filename);
};


#endif //MINIGIT_REPOSITORY_H
