//
// Created by nofilkhan on 7/25/26.
//
#include "CommandParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
	if (argc < 2){
		std::cerr << "usage: mingit <command> [args...]\n";
		return 1;
	}
	return CommandParser::run(argc, argv);

}
