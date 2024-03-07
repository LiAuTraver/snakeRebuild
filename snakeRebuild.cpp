#include <csignal>
import std.compat;
import ancillarycat.handler;
import ancillarycat.print;



int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	handler::oninitialize();
	std::string title = "Snake";
	consolePrintln
		.centered("\n\n\n")
		.centered(title)
		.centered("\n\n\n")
		.centered("")
		.centered("Press 1 to start")
		.centered("")
		.centered("Press 2 to check the leaderboard")
		.centered("")
		.centered("Press Ctrl+C to exit");

		// Register signal and signal handler
		signal(SIGINT, handler::signalHandler);
	std::cin.get();
	return 0;
}
