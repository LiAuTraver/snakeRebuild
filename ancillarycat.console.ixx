module;

#include <Windows.h>
#include <conio.h>

export module ancillarycat.console;

import std;

export class Console {
	friend class ConsolePrint;
	friend class ConsolePrintln;
public:
	explicit Console(const SHORT minWidth = 0, const SHORT minHeight = 0)
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
		if (width < minWidth || height <= minHeight) {
			throw std::runtime_error("Console width and height must be not less than the given minimum width and height");
		}
	}
	// cursor control
public:
	inline Console& setCursorState(const bool state = true)
	{
		cursorInfo_.bVisible = state;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
		return *this;
	}
	inline Console& getCursorCoordinate()
	{
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
		cursorCoordinate = consoleScreenBufferInfo_.dwCursorPosition;
		cursorRow = cursorCoordinate.Y;
		cursorCol = cursorCoordinate.X;
		return *this;
	}
	inline Console& setCursorCoordinate(const SHORT row = cursorRow, const SHORT col = cursorCol)
	{
		cursorCoordinate.X = col;
		cursorCoordinate.Y = row;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
		return *this;
	}
	inline Console& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0)
	{
		cursorCoordinate.Y += deltaRow;
		cursorCoordinate.X += deltaCol;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
		return *this;
	}
	// output attributes control
public:
	inline Console& resetAnsi()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return *this;
	}
	inline Console& setAnsi(const WORD attribute)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
		return *this;
	}
	// console interaction
public:
	static inline CHAR&& getch() noexcept {
		return std::move(_getch());
	}
	// public access
public:
	SHORT width{};
	SHORT height{};
	inline static COORD cursorCoordinate{};
	inline static SHORT cursorRow{};
	inline static SHORT cursorCol{};
private:
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo_{};
	HANDLE handleStdin_{};
	HANDLE handleStdout_{};
	DWORD stdinMode_{};
	DWORD stdoutMode_{};
	CONSOLE_CURSOR_INFO cursorInfo_{};
}console;