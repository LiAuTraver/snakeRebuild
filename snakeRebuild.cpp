import "config.hpp";

import ancillarycat.console;

int main(int argc, char* argv[]) {
	while (true)
		// zero to exit, non-zero to restart or else
		switch (realMain(argc, argv)) {
		case EXIT_PROGRAM:
			return EXIT_PROGRAM;
			[[unreachable]] break;
		case RESTART_PROGRAM:
			// TODO: re-initialize the console height and width
			console.terminalSizeChange();
			continue;
			[[unreachable]] break;
		case UNKNOWN_ERROR:
			return UNKNOWN_ERROR;
			[[unreachable]] break;
		default:
#define INT_MAX 2147483647
			[[unlikely]] return INT_MAX;
			break;
		}
#define INT_MIN -2147483648
	[[unreachable]] return INT_MIN;
}