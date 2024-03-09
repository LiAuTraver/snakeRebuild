module;

#include "config.hpp"

export module ancillarycat.panel;

import ancillarycat.print;
import ancillarycat.ansi;
import ancillarycat.console;
import std;

NO_EXPORT const static std::vector<std::string> SNAKE{
	" ____              _        ",
	"/ ___| _ __   __ _| | _____ ",
	"\\___ \\| '_ \\ / _` | |/ / _ \\",
	" ___) | | | | (_| |   <  __/",
	"|____/|_| |_|\\__,_|_|\\_\\___|"
};

namespace panel
{
export void showMenu()
{
	consolePrint.moveCursor(3, 0);
	std::ranges::for_each(SNAKE, [](const std::string& str) {
		consolePrintln.centered(str);
		});
	consolePrintln
		.moveCursor(3, 0)
		.centered("Snake")
		.moveCursor(1, 0)
		.centered("Press 1 to start", ansiColor::green)
		.moveCursor(1, 0)
		.centered("Press 2 to check the leaderboard", ansiColor::red)
		.moveCursor(1, 0)
		.centered("Press 3 to configure settings", ansiColor::blue)
		.moveCursor(1, 0)
		.centered("Press 'q' or 'Q' to exit", ansiColor::yellow)
		.setStyle()
		.getCursor();
}
}