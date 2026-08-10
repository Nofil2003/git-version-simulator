//Created on 9/8/26 on Nofil Khan
//

#include "Index.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

bool Index::add(const std::string& filename){

	if (!Utils::pathExists(filename)){
		std::cerr << "file not found: " << filename << "\n";
		return false;
	}
	std::string current = Utils::readFile(".mygit/index");
	
	std::vector<std::string> staged = Index::getStagedFiles();
	for (const std::string& f : staged) {
    		if (f == filename) {
        		std::cout << "already staged\n";
        		return true;
    		}
	}

	current += filename + "\n";

	Utils::writeFile(".mygit/index", current);
	return true;
}

std::vector<std::string> getStagedFiles(){

}

void Index::clear(){
	
}
