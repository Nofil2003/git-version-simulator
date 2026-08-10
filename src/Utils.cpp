//
// Created by nofilkhan on 7/25/26.
//

#include "Utils.h"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <ctime>

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

	// Copy a file to another path, replacing whatever was there before.
	bool copyFile(const std::string& from, const std::string& to){
		std::error_code ec;
		fs::copy_file(from, to, fs::copy_options::overwrite_existing, ec);
		return !ec;
	}

	// Return the regular files directly inside dir. The repo folder itself is
	// skipped so status/commit never track minigit's own metadata.
	std::vector<std::string> listFiles(const std::string& dir){
		std::vector<std::string> files;
		std::error_code ec;
		for (const auto& entry : fs::directory_iterator(dir, ec)) {
			if (!entry.is_regular_file()) continue;
			std::string name = entry.path().filename().string();
			if (name == ".mygit") continue;
			files.push_back(name);
		}
		return files;
	}

	// Current local time formatted for storage inside commit files.
	std::string timestamp(){
		std::time_t now = std::time(nullptr);
		char buf[32];
		std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
		return std::string(buf);
	}
}
