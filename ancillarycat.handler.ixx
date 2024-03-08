module;

#include <csignal>
#include <Windows.h>
#include <mmsystem.h>
#include "config.hpp"
#pragma warning (disable: 4706)
#pragma warning (disable: 5030)
#pragma warning (disable: 5222)

#pragma comment(lib, "winmm.lib")

export module ancillarycat.handler;

import ancillarycat.console;
import ancillarycat.print;
import ancillarycat.snake;
import ancillarycat.leaderboard;
import ancillarycat.config;
import ancillarycat.ansi;
import ancillarycat.windows.api;

namespace handler {
export inline void oninitialize() {
	system("CLS");
	console.setCursorState(true);
	api::initializeSoundEvent();
	api::soundEvent(L"\\Media\\Alarm10.wav");
}

NO_EXPORT static void exitProgram(const int& exitCode = 0) {
	console
		.setCursorState(true)
		.setCursorCoordinate(0, 0);
	system("CLS");
	exit(exitCode);
}
// cannot export a static function so I removed it.
// cannot make signalNumber const or reference, dunno why.
export void signalHandler(const int signalNumber) {
	switch (signalNumber)
	{
	case SIGINT:
		handler::exitProgram();
		[[unreachable]]
		break;
	default: [[unlikely]]
			   break;
	}
}

NO_EXPORT static void defaultHandler() {
	consolePrintln
		.moveCursor(1, 0)
		.setStyle(ansiStyle::bold)
		.centered("invalid key.", ansiColor::red, ansiBackground::yellowIntense)
		.setCursor(console.cursorRow, 0);
	api::soundEvent(L"");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	consolePrintln
		.fillLine()
		.setCursor(console.cursorRow - 2, 0);
}

NO_EXPORT static inline void handleCBreak() {
	// Save the original signal handler
	const _crt_signal_t originalHandler = signal(SIGINT, SIG_IGN);

	consolePrintln
		.setStyle(ansiStyle::blink)
		.moveCursor(1, 0)
		.centered("Ctrl + C does not work here :P", ansiColor::white, ansiBackground::red)
		.setStyle(ansiStyle::reverse)
		.moveCursor(1, 0)
		.centered("Note: Press q or Q to quit", ansiColor::white, ansiBackground::red)
		.setCursor(console.cursorRow - 2, 0);
	api::soundEvent(L"\\Media\\Windows Logon.wav");
	std::this_thread::sleep_for(std::chrono::seconds(3));

	consolePrintln
		.fillLine(' ', 4)
		.setCursor(console.cursorRow - 5, 0);

	// Restore the original signal handler
	signal(SIGINT, originalHandler);
}

export void option() {
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
			handler::handleCBreak();
			break;
		case 'q':
			[[fallthrough]];
		case 'Q':
			handler::exitProgram();
			[[unreachable]]
			break;
		default:
			defaultHandler();
			break;
		}
	}
}

} // namespace handler

#pragma warning (default: 4706)
#pragma warning (default: 5030)
#pragma warning (default: 5222)
