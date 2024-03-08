#include <csignal>

import std;
import ancillarycat.handler;
import ancillarycat.panel;
import ancillarycat.leaderboard;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	handler::oninitialize();
	panel::showMenu();
	game::leaderboardInit();
	handler::option();
	// initialize leaderboard
	// Register signal and signal handler, ignore SIGINT here
	signal(SIGINT, SIG_IGN);

	std::cin.get();
	return 0;
}