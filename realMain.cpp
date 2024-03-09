#include "config.hpp"
#include <csignal>

import std;
import ancillarycat.handler;
import ancillarycat.panel;
import ancillarycat.leaderboard;

// cannot make static because static function can only be called in the same file.
inline int realMain([[maybe_unused]] const int& argc, [[maybe_unused]] char* argv[])
{
	handler::oninitialize();
	panel::showMenu();
	game::leaderboardInit();
	handler::option();
	// initialize leaderboard
	// Register signal and signal handler, ignore SIGINT here
	signal(SIGINT, SIG_IGN);

	std::cin.get();
	return EXIT_PROGRAM;
}