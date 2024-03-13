module;
export module ancillarycat.leaderboard;

import "config.hpp";
import std;
import ancillarycat.console;
import ancillarycat.ansi;
import ancillarycat.windows.api;
import <Windows.h>;

namespace game
{
export void leaderboardInit();
export const std::chrono::seconds timer(const std::chrono::milliseconds& duration = std::chrono::milliseconds(875));
export std::chrono::milliseconds elapsed(0);

}
namespace game
{
NO_EXPORT static bool writeAccess = true;
// create/open a file to store the leaderboard
DEFINITION inline void leaderboardInit() {
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

DEFINITION inline const std::chrono::seconds timer(const std::chrono::milliseconds& duration)
{
	std::this_thread::sleep_for(duration);
	elapsed += duration;
	// cast milli to sec
	return std::chrono::duration_cast<std::chrono::seconds>(elapsed);
}

DEFINITION inline void writeDown(const std::string_view name, const short& score, const long double& time, const std::chrono::seconds& elapsed)
{
	if (writeAccess) {
		std::ofstream file("leaderboard.snake", std::ios::app);
		file << name << " " << score << " " << time << " " << elapsed.count() << std::endl;
		file.close();
	}
	// TODO: to be polished
	//else {
	//	console
	//		.setStyle(ansiStyle::blink)
	//		.centered("Error: Unable to write to leaderboard file", ansiColor::red, ansiBackground::black);
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
}
}

