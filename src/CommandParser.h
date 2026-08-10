//
// Created by nofilkhan on 7/25/26.
//

#ifndef MINIGIT_COMMANDPARSER_H
#define MINIGIT_COMMANDPARSER_H


class CommandParser {
	public:
		// Route argv[1] to the matching Repository command.
		static int run(int argc, char* argv[]);
		// Print the supported commands.
		static void printUsage();
		// Print the project landing page (author info + about + usage).
		static void printLanding();
};


#endif //MINIGIT_COMMANDPARSER_H
