module;
// between `module;` and `export module ancillarycat.panel;` is called `global module fragment`, which can only contain preprocessor directives and import/export declarations.

export module ancillarycat.panel;

import ancillarycat.print;
import ancillarycat.ansi;
import ancillarycat.console;
import std;

const static std::vector<std::string> SNAKE{
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
	std::ranges::for_each(SNAKE, [](const std::string& str) {
		consolePrintln.centered(str);
		});
	consolePrintln
		.moveCursor(3, 0)
		.centered("Snake")
		.setFontColor(ansiColor::green).moveCursor(1, 0)
		.centered("Press 1 to start")
		.setFontColor(ansiColor::red).moveCursor(1, 0)
		.centered("Press 2 to check the leaderboard")
		.setFontColor(ansiColor::blue).moveCursor(1, 0)
		.centered("Press 3 to configure settings")
		.setFontColor(ansiColor::yellow).moveCursor(1, 0)
		.centered("Press Ctrl+C to exit")
		.reset()
		.updateCursor();
	/*for (auto i = 0; i < console.height - console.cursorRow - 5; i++)
	{
		consolePrintln.fillLine();
	}*/
}
}