module;
#include "../include/config.hpp";
export module ancillarycat.handler;

namespace handler
{
	NO_EXPORT void defaultHandler();
	NO_EXPORT void handleCBreak();
	NO_EXPORT int showInvalidConsoleSize();
	NO_EXPORT int checkConsoleSize();
	NO_EXPORT int showMenu();
	NO_EXPORT int option();
	export void signalHandler(const int signalNumber);
	export int oninitialize();
}


