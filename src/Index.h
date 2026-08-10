//created on 9/8/2026 by Nofil Khan
//
#pragma once
#include <string>
#include <vector>

class Index{
public:
	static bool add(const std::string& filename);
	static std::vector<std::string> getStagedFiles();
	static void clear();
};
