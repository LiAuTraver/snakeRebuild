#include "config.hpp"

import ancillarycat.console;
import ancillarycat.print;

int main(int argc, char* argv[]) {
	while (true)
		// zero to exit, non-zero to restart or else
		switch (realMain(argc, argv)) {
		case EXIT_PROGRAM:
			return EXIT_PROGRAM;
			[[unreachable]]break;
		case RESTART_PROGRAM:
			// TODO: re-initialize the console height and width
			continue;
			[[unreachable]]break;
		default:
			// currently
			[[unreachable]] return INT_MAX;
			break;
		}

	[[unreachable]] return INT_MIN;
}