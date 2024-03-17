module;
#include "../include/config.hpp"
#pragma comment(lib, "winmm.lib")
#include <csignal>
#include <Windows.h>
module ancillarycat.handler;
import ancillarycat.console;
import ancillarycat.api;
import ancillarycat.game;
import std;
namespace handler
{
inline int checkConsoleSize() {
	if (console.height >= BOX_HEIGHT && console.width >= BOX_WIDTH)
		return VALID;
	else
		return INVALID;
}
inline int showInvalidConsoleSize() {
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

	while (const int ch = console.getch()) {
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
	std::unreachable();
	//return UNKNOWN_ERROR;
}
inline void defaultHandler() {
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
inline void handleCBreak() {
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
	api::soundEvent(LR"(\Media\Windows Logon.wav)");
	std::this_thread::sleep_for(std::chrono::seconds(3));

	console
		.fillLine(' ', 4)
		.setCursorCoordinate(console.height - 5, 0);

	// Restore the original signal handler
	signal(SIGINT, originalHandler);
}
inline void signalHandler(const int signalNumber) {
	switch (signalNumber)
	{
	case SIGINT:
		handler::handleCBreak();
		//std::unreachable();
		break;
	case SIGABRT:
		// TODO: implement the handler for abort signal
		//std::unreachable();
		break;
	case SIGFPE:
		// TODO: implement the handler for 
		//std::unreachable();
		break;
	case SIGILL:
		// TODO
		//std::unreachable();
		break;
	case SIGSEGV:
		// TODO
		//std::unreachable();
		break;
	case SIGTERM:
		// TODO
		//std::unreachable();
		break;
	case SIGBREAK:
		// TODO
		//std::unreachable();
		break;
	default: [[unlikely]]
			   // TODO
			   break;
	}
}
inline int option() {
	while (const int ch = console.getch()) {
		switch (ch)
		{
		case '1':
			signal(SIGINT, handler::signalHandler);
			game::snakeGame();
			break;
		case '2':
			game::snakeLeaderboard();
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
			//std::unreachable();
		case 'r':
			[[fallthrough]];
		case 'R':
			return RESTART_PROGRAM;
			//std::unreachable();
		default:
			defaultHandler();
			break;
		}
		console.clear();
		handler::showMenu();
	}
	std::unreachable();
	//return UNKNOWN_ERROR;
}
inline int showMenu()
{
	console.moveCursor(3, 0);
	std::ranges::for_each(SNAKE, [](const std::string& str) {
		console.centered(str);
		});
	console
		.moveCursor(3, 0)
		.setStyle(ansiStyle::blink)
		.centered("Snake", ansiColor::whiteIntense)
		.resetAnsi()
		.moveCursor(1, 0)
		.centered("Press 1 to start", ansiColor::green)
		.moveCursor(1, 0)
		.centered("Press 2 to check the leaderboard", ansiColor::redIntense)
		.moveCursor(1, 0)
		.centered("Press 3 to configure settings", ansiColor::blue)
		.moveCursor(1, 0)
		.centered("Press 'q' or 'Q' to exit", ansiColor::yellow)
		.moveCursor(1, 0)
		.centered("Press 'r' or 'R' to restart", ansiColor::magenta)
		.resetAnsi()
		.getCursorCoordinate();
	return VALID;
}
inline int oninitialize() {
	if (handler::checkConsoleSize() == INVALID) {
		if (handler::showInvalidConsoleSize() == RESTART_PROGRAM)
			console.bot("Restarting the program...", ansiColor::yellow, ansiBackground::blueIntense);
		return RESTART_PROGRAM;
		return EXIT_PROGRAM;
	}
	console.clear();
	api::initializeSoundEvent();
	api::soundEvent(LR"(\Media\Alarm10.wav)");
	handler::showMenu();
	game::leaderboardInit();
	// Register signal and signal handler, ignore SIGINT here
	signal(SIGINT, SIG_IGN);
	return handler::option();
}
} // namespace handler