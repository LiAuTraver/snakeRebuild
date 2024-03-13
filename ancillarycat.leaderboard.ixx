module;


export module ancillarycat.leaderboard;

import "config.hpp";
import std;
import ancillarycat.console;
import ancillarycat.ansi;

namespace game
{
NO_EXPORT static bool writeAccess = true;
// create/open a file to store the leaderboard
export inline void leaderboardInit() {
	std::ifstream checkFile("leaderboard.snake", std::ios::in);
	//check whether the file exists
	if (checkFile.fail()) {
		checkFile.close();
		std::ofstream file("leaderboard.snake");
		if (file.fail()) {
			writeAccess = false;
			console
				.setStyle(ansiStyle::blink)
				.centered("Error: Unable to create leaderboard file", ansiColor::red, ansiBackground::black);
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		file.close();
	}
}

}