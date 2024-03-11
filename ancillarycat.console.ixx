module;

#include "config.hpp"
#include <Windows.h>
#include <conio.h>

export module ancillarycat.console;

import std;

export class Console {
public:
	explicit Console(const SHORT& minWidth = MIN_WIDTH, const SHORT& minHeight = MIN_HEIGHT) : cursorRow(0),cursorCol(0),cursorCoordinate({0,0})
	{
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
		handleStdin_ = GetStdHandle(STD_INPUT_HANDLE);  // Get the standard input handle
		GetConsoleMode(handleStdin_, &stdinMode_);
		stdinMode_ &= ~ENABLE_ECHO_INPUT;
		stdinMode_ &= ~ENABLE_LINE_INPUT;
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
	}
	// cursor control
public:
	Console& setCursorState(const bool state = true)
	{
		cursorInfo_.bVisible = state;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
		return *this;
	}
	Console& getCursorCoordinate()
	{
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
		cursorCoordinate = consoleScreenBufferInfo_.dwCursorPosition;
		cursorRow = cursorCoordinate.Y;
		cursorCol = cursorCoordinate.X;
		return *this;
	}
	Console& setCursorCoordinate(const SHORT row = DEFAULT_VAL, const SHORT col = DEFAULT_VAL)
	{
		cursorCoordinate.X = col;
		cursorCoordinate.Y = row;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
		return this->getCursorCoordinate();
	}
	Console& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0)
	{
		cursorCoordinate.Y += deltaRow;
		cursorCoordinate.X += deltaCol;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
		return this->getCursorCoordinate();
	}
	// output attributes control
public:
	Console& resetAnsi()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return *this;
	}
	Console& setAnsi(const WORD attribute)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
		return *this;
	}
	// console interaction
public:
	static int getch() noexcept {
		return _getch();
	}
	Console& box(const SHORT& curRow = 0, const SHORT& curCol = 0, SHORT boxHeight = DEFAULT_VAL, SHORT boxWidth = DEFAULT_VAL, const char& border = '+', const char& horizontal = '-', const char& vertical = '|')
	{
		if (boxHeight == DEFAULT_VAL)boxHeight = this->height;
		if (boxWidth == DEFAULT_VAL)boxWidth = this->width;
		this->setCursorCoordinate(curRow, curCol);
		std::print("{}", border);
		for (SHORT i = 0; i < boxWidth - 2; i++) {
			std::print("{}", horizontal);
		}
		std::println("{}", border);
		this->setCursorCoordinate(curRow + 1, curCol);
		for (SHORT i = 0; i < boxHeight - 1; i++) {
			std::print("{}", vertical);
			this->moveCursor(0, boxWidth - 1);
			std::print("{}", vertical);
			this->moveCursor(1, -boxWidth + 1);
		}
		std::print("{}", border);
		for (SHORT i = 0; i < boxWidth - 2; i++) {
			std::print("{}", horizontal);
		}
		std::print("{}", border);
		return this->setCursorCoordinate(curRow + 1, curCol + 1)
			.getCursorCoordinate();
	}
	// public access
public:
	SHORT width;
	SHORT height;
	COORD cursorCoordinate;
	SHORT cursorRow;
	SHORT cursorCol;
private:
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo_{};
	HANDLE handleStdin_{};
	HANDLE handleStdout_{};
	DWORD stdinMode_{};
	DWORD stdoutMode_{};
	CONSOLE_CURSOR_INFO cursorInfo_{};
public:
	// use when the program `restarts` or `resumes` from a `paused` state
	Console& terminalSizeChange() {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
		width = consoleScreenBufferInfo_.srWindow.Right - consoleScreenBufferInfo_.srWindow.Left + 1;
		height = consoleScreenBufferInfo_.srWindow.Bottom - consoleScreenBufferInfo_.srWindow.Top + 1;
		return *this;
	}
}console;