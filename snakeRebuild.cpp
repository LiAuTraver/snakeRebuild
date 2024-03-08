#include <csignal>

import std;
import ancillarycat.handler;
import ancillarycat.panel;
import ancillarycat.console;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	handler::oninitialize();
	panel::showMenu();
	console.setCursorState(false);
	handler::option();
	// Register signal and signal handler
	signal(SIGINT, handler::signalHandler);
	std::cin.get();
	return 0;
}
