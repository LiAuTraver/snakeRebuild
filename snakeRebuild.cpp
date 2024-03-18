#include "./include/config.hpp"
import ancillarycat.console;
import std;
int main(const int argc, char* argv[]) {
	while (true)
		// zero to exit, non-zero to restart or else
		switch (realMain(argc, argv)) {
		case EXIT_PROGRAM:
			return EXIT_PROGRAM;
			//std::unreachable();
		case RESTART_PROGRAM:
			// TODO: re-initialize the console height and width
			console.oninitialize();
			continue;
			//std::unreachable();
		case UNKNOWN_ERROR:
			return UNKNOWN_ERROR;
			//std::unreachable();
		default:
			[[unlikely]] return INT_MAX;
			//std::unreachable();
		}
	std::unreachable();
	//return INT_MIN;
}