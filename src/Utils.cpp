//
// Created by nofilkhan on 7/25/26.
//

#include "Utils.h"
#include <filesystem>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;
namespace Utils{

	bool pathExists(const std::string &path){
		return fs::exists(path);
	}

	bool createDir(const std::string &path){
		if(fs::exists(path)) return true;
		return fs::create_directories(path);
	}

	bool writeFile(const std::string &path, const std::string &content){
		std::ofstream out(path);
		if (!out) return false;
		out << content;
		return true;
	}

	std::string readFile(const std::string& path) {
		std::ifstream in(path);
		if (!in) return "";
		std::stringstream ss;
		ss << in.rdbuf();
		return ss.str();
	}
}
