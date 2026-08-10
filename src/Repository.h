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
	static int commit(const std::string& message);
	static int log();
	static int status();
	static int checkout(const std::string& commitId);
};


#endif //MINIGIT_REPOSITORY_H
