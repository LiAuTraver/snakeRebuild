#include "../include/config.hpp"

import ancillarycat.handler;
// for historical reason, this `realMain` function was preserved as the main entry point of the program.

// cannot make static because static function can only be called in the same file.
int realMain([[maybe_unused]] const int& argc, [[maybe_unused]] char* argv[]){
	return handler::oninitialize();
}