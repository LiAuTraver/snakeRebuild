module;

#include <csignal>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

export module ancillarycat.handler;

import ancillarycat.console;
import ancillarycat.print;
import ancillarycat.panel;
import ancillarycat.snake;
import ancillarycat.leaderboard;
import ancillarycat.config;
import std;

namespace handler {

// cannot export a static function so I removed it.
	// cannot make signalNumber const or reference, dunno why.
export void signalHandler(int signalNumber) {
	switch (signalNumber)
	{
	case SIGINT:
		system("CLS");
		exit(0);
	default: [[unlikely]]
			   break;
	}
}
export void defaultHandler()
{
	consolePrintln
		.centered("You pressed an invalid key")
		.centered("Press any key to continue");
	PlaySound(TEXT("%SystemRoot%\\Media\\Windows Logon.wav"), nullptr, SND_FILENAME);
	console
		.moveCursor(0, 0);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	system("CLS");
	panel::showMenu();
}
export void oninitialize() {
	system("CLS");
}

export void option()
{
	int ch;
	while (ch = console.getch()) {
		switch (ch)
		{
		case '1':
			game::snakeGame();
			break;
		case '2':
			game::leaderboard();
			break;
		case '3':
			game::config();
			break;
		case 3: // Ctrl+C
			consolePrintln.rightAligned("You pressed Ctrl+C");
			signalHandler(3);
			break;
		default:
			defaultHandler();
			break;
		}
	}
}

} // namespace handler
