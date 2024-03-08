module;

#include "config.hpp"

export module ancillarycat.leaderboard;

import ancillarycat.print;
import ancillarycat.ansi;
import std;

namespace game
{
// create/open a file to store the leaderboard
export inline void leaderboardInit() {
	std::ifstream checkFile("leaderboard.snake", std::ios::in);
	//check whether the file exists
	if (checkFile.fail()) {
		checkFile.close();
		std::ofstream file("leaderboard.snake");
		if (file.fail()) {
			writeAccess = false;
			consolePrintln
				.setStyle(ansiStyle::blink)
				.centered("Error: Unable to create leaderboard file", ansiColor::red, ansiBackground::black);
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		file.close();
	}
}

NO_EXPORT static bool writeAccess = true;
}