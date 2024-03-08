module;

#include <Windows.h>

export module ancillarycat.console;

import std;
import ancillarycat.print;

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
		if (width < minWidth || height <= minHeight)
		{
			throw std::runtime_error("Console width and height must be not less than the given minimum width and height");
		}
	}
public:
	inline Console& setCursorState(const bool state = true)
	{
		cursorInfo_.bVisible = state;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
		return *this;
	}
	inline Console& setCursorPosition(const SHORT x, const SHORT y)
	{
		COORD coord = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		return *this;
	}
	inline Console& resetAnsi()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return *this;
	}
public:
	// public access
	SHORT width;
	SHORT height;
private:
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo_;
	HANDLE handleStdin_{};
	HANDLE handleStdout_{};
	DWORD stdinMode_{};
	DWORD stdoutMode_{};
	CONSOLE_CURSOR_INFO cursorInfo_{};
}console;