#include <csignal>

import std;
import ancillarycat.handler;
import ancillarycat.panel;

void terminalSizeChange();

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	handler::oninitialize();
	panel::showMenu();
	handler::option();
	// Register signal and signal handler
	signal(SIGINT, handler::signalHandler);

	std::cin.get();
	return 0;
}

void terminalSizeChange()
{

}