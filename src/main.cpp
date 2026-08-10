//
// Created by nofilkhan on 7/25/26.
//
#include "CommandParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
	// No command given: show the landing page instead of an error.
	if (argc < 2){
		CommandParser::printLanding();
		return 0;
	}
	return CommandParser::run(argc, argv);

}
