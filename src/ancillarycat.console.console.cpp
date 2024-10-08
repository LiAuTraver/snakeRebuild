module;

#include <Windows.h>
#include "../include/config.hpp"
#include <conio.h>

module ancillarycat.console:console;

import ancillarycat.api;
import std;

Console::Console() : cursorCoordinate({ 0, 0 }) {
	this->oninitialize();
}

// use when the program `restarts` or `resumes` from a `paused` state
Console& Console::oninitialize() noexcept {
	return this->clear()
		.reset(true);
}

Console& Console::reset(const bool& invisibility) noexcept {
	if (invisibility) {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
		handleStdin_ = GetStdHandle(STD_INPUT_HANDLE);  // Get the standard input handle
		GetConsoleMode(handleStdin_, &stdinMode_);
		stdinMode_ &= ~ENABLE_ECHO_INPUT;
		stdinMode_ &= ~ENABLE_LINE_INPUT;
		stdinMode_ |= ENABLE_WINDOW_INPUT;  // Enable window input
		SetConsoleMode(handleStdin_, stdinMode_);
		handleStdout_ = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(handleStdout_, &stdoutMode_);
		stdoutMode_ |= ENABLE_PROCESSED_OUTPUT;
		stdoutMode_ |= ENABLE_WRAP_AT_EOL_OUTPUT;
		SetConsoleMode(handleStdout_, stdoutMode_);
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
		cursorInfo_.bVisible = false;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
		width = consoleScreenBufferInfo_.srWindow.Right - consoleScreenBufferInfo_.srWindow.Left + 1;
		height = consoleScreenBufferInfo_.srWindow.Bottom - consoleScreenBufferInfo_.srWindow.Top + 1;
		return *this;
	}
	else {
		// Restore the cursor visibility
		cursorInfo_.bVisible = true;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);

		// Reset standard input mode to its original state
		stdinMode_ |= ENABLE_ECHO_INPUT;  // Enable echo input
		stdinMode_ |= ENABLE_LINE_INPUT;  // Enable line input
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), stdinMode_);

		//stdoutMode_ &= ~ENABLE_PROCESSED_OUTPUT;
		//stdoutMode_ &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
		//SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), stdoutMode_);

		return *this;
	}
}


// cursor control
Console& Console::setCursorState(const bool& state) noexcept {
	cursorInfo_.bVisible = state;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
	return *this;
}

Console& Console::getCursorCoordinate() noexcept {
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
	cursorCoordinate = consoleScreenBufferInfo_.dwCursorPosition;
	cursorRow = cursorCoordinate.Y;
	cursorCol = cursorCoordinate.X;
	return *this;
}

Console& Console::moveCursor(const SHORT deltaRow, const SHORT deltaCol) {
	cursorCoordinate.Y += deltaRow;
	cursorCoordinate.X += deltaCol;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
	return this->getCursorCoordinate();
}

// output attributes control
Console& Console::resetAnsi() noexcept {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return *this;
}

Console& Console::setAnsi(const WORD attribute) noexcept {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
	return *this;
}

// console interaction
int Console::getch(const bool& notNeedRealKey) noexcept {
	return notNeedRealKey ? :: _kbhit() : ::_getch();
}

Console& Console::box(const SHORT& curRow, const SHORT& curCol, SHORT boxHeight, SHORT boxWidth, const char& border,
	const char& horizontal, const char& vertical) {
	if (boxHeight == DEFAULT_VAL) boxHeight = this->height;
	if (boxWidth == DEFAULT_VAL) boxWidth = this->width;
	this->setCursorCoordinate(curRow - 1, curCol);
	std::print("{}", border);
	for (SHORT i = 0; i < boxWidth; i++) {
		std::print("{}", horizontal);
	}
	std::println("{}", border);
	for (SHORT i = 0; i < boxHeight; i++) {
		this->setCursorCoordinate(curRow + i, curCol);
		std::print("{}", vertical);
		this->setCursorCoordinate(curRow + i, curCol + boxWidth + 1);
		std::print("{}", vertical);
	}
	this->setCursorCoordinate(curRow + boxHeight, curCol);
	std::print("{}", border);
	for (SHORT i = 0; i < boxWidth; i++) {
		std::print("{}", horizontal);
	}
	std::print("{}", border);
	return this->setCursorCoordinate(curRow, curCol);
}

Console& Console::setStyle(const ansiStyle& style) noexcept {
	std::print("\033[{}m",
		static_cast<int>(style));
	return this->getCursorCoordinate();
}

Console& Console::fillLine(const char& fillChar, const int& dRow, const bool& newline) noexcept {
	if (dRow < 1)
		return *this;
	// return to the beginning of the line
	std::string line(width, fillChar);
	for (int count = 0; count < dRow; count++) {
		std::print("\r\033[K\033[{}m{}\033[{}m", static_cast<int>(ansiStyle::reset), line,
			static_cast<int>(ansiStyle::reset));
	}
	if (newline)std::println("");
	return this->getCursorCoordinate();
}
Console& Console::set_cur_coor_impl(const SHORT& row, const SHORT& col) noexcept {
	cursorCoordinate.Y = row;
	cursorCoordinate.X = col;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
	return *this;
}

Console& Console::clear() noexcept {
	DWORD count;
	resetAnsi();
	FillConsoleOutputCharacter(handleStdout_, L' ', width * height, { 0, 0 }, &count);
	FillConsoleOutputAttribute(handleStdout_, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, width * height,
		{ 0, 0 }, &count);
	return this->setCursorCoordinate(0, 0);
}