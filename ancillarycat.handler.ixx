module;


#pragma warning (disable: 4706)
#pragma warning (disable: 5030)
#pragma warning (disable: 5222)

#pragma comment(lib, "winmm.lib")

export module ancillarycat.handler;

import "config.hpp";
import <csignal>;
import <Windows.h>;
import ancillarycat.console;
import ancillarycat.snake;
import ancillarycat.leaderboard;
import ancillarycat.config;
import ancillarycat.ansi;
import ancillarycat.windows.api;
import ancillarycat.panel;
import std;

namespace handler {

NO_EXPORT inline int checkConsoleSize() {
	if (console.height >= BOX_HEIGHT && console.width >= BOX_WIDTH)
		return VALID;
	else
		return INVALID;
}

NO_EXPORT inline int showInvalidConsoleSize() {
	console
		.setStyle(ansiStyle::bold)
		.centered("Console size is too small.", ansiColor::red, ansiBackground::yellowIntense)
		.centered("You may need to resize the terminal size.", ansiColor::red, ansiBackground::yellowIntense)
		.setStyle()
		.centered("After resizing the terminal size, press 'r' or 'R' to restart program,", ansiColor::white, ansiBackground::red)
		.centered("or press 'q' or 'Q' to quit.", ansiColor::white, ansiBackground::red)
		.setCursorCoordinate(console.cursorRow, 0);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	console
		.fillLine()
		.setCursorCoordinate(console.cursorRow - 2, 0);

	while (int ch = console.getch()) {
		switch (ch) {
		case 'r':
			[[fallthrough]];
		case 'R':
			return RESTART_PROGRAM;
		case 'q':
			[[fallthrough]];
		case 'Q':
			return EXIT_PROGRAM;
		default:
			break;
		}
	}
	[[unreachable]] return UNKNOWN_ERROR;
}


NO_EXPORT static void defaultHandler() {
	console
		.moveCursor(1, 0)
		.setStyle(ansiStyle::bold)
		.centered("invalid key.", ansiColor::red, ansiBackground::yellowIntense)
		.setCursorCoordinate(console.cursorRow, 0);
	api::soundEvent();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	console
		.fillLine()
		.setCursorCoordinate(console.cursorRow - 2, 0);
}

NO_EXPORT static inline void handleCBreak() {
	// Save the original signal handler
	const _crt_signal_t originalHandler = signal(SIGINT, SIG_IGN);

	console
		.setStyle(ansiStyle::blink)
		.setCursorCoordinate(console.height - 5, 0)
		.moveCursor(1, 0)
		.centered("Ctrl + C does not work here :P", ansiColor::white, ansiBackground::red)
		.setStyle(ansiStyle::reverse)
		.moveCursor(1, 0)
		.centered("Note: Press q or Q to quit", ansiColor::white, ansiBackground::red)
		.setCursorCoordinate(console.height - 4, 0);
	api::soundEvent(L"\\Media\\Windows Logon.wav");
	std::this_thread::sleep_for(std::chrono::seconds(3));

	console
		.fillLine(' ', 4)
		.setCursorCoordinate(console.height - 5, 0);

	// Restore the original signal handler
	signal(SIGINT, originalHandler);
}

// cannot export a static function so I removed it.
// cannot make signalNumber const or l/r value reference
export void signalHandler(const int signalNumber) {
	switch (signalNumber)
	{
	case SIGINT:
		handler::handleCBreak();
		[[unreachable]] break;
	case SIGABRT:
		// TODO: implement the handler for abort signal
		[[unreachable]] break;
	case SIGFPE:
		// TODO: implement the handler for 
		[[unreachable]] break;
	case SIGILL:
		// TODO
		[[unreachable]] break;
	case SIGSEGV:
		// TODO
		[[unreachable]] break;
	case SIGTERM:
		// TODO
		[[unreachable]] break;
	case SIGBREAK:
		// TODO
		[[unreachable]] break;
	default: [[unlikely]]
			   // TODO
			   break;
	}
}

NO_EXPORT int option() {
	while (const int ch = console.getch()) {
		switch (ch)
		{
		case '1':
			signal(SIGINT, handler::signalHandler);
			game::snakeGame();
			break;
		case '2':
			game::leaderboardInit();
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
			return EXIT_PROGRAM;
			[[unreachable]]
			break;
		case 'r':
			[[fallthrough]];
		case 'R':
			return RESTART_PROGRAM;
			[[unreachable]]
			break;
		default:
			defaultHandler();
			break;
		}
	}
	[[unreachable]] return UNKNOWN_ERROR;
}

export inline int oninitialize() {
	if (handler::checkConsoleSize() == INVALID) {
		if (handler::showInvalidConsoleSize() == RESTART_PROGRAM)
			return RESTART_PROGRAM;
		return EXIT_PROGRAM;
	}
	system("CLS");
	console.setCursorState(true);
	api::initializeSoundEvent();
	api::soundEvent(L"\\Media\\Alarm10.wav");
	panel::showMenu();
	game::leaderboardInit();
	// Register signal and signal handler, ignore SIGINT here
	signal(SIGINT, SIG_IGN);
	return handler::option();
}
} // namespace handler
#pragma warning (default: 4706)
#pragma warning (default: 5030)
#pragma warning (default: 5222)
