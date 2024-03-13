module;
export module ancillarycat.impl.console;

import "config.hpp";
import <Windows.h>;
import <conio.h>;
import ancillarycat.ansi;
import std;

export class Console final {
public:
	explicit Console(const SHORT& minWidth = MIN_WIDTH, const SHORT& minHeight = MIN_HEIGHT) : cursorRow(0), cursorCol(0), cursorCoordinate({ 0,0 })
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
		this->getCursorCoordinate();
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
	Console& resetAnsi() noexcept
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return *this;
	}
	Console& setAnsi(const WORD attribute) noexcept
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
		return *this;
	}
	// console interaction
public:
	static const int getch() noexcept {
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
	// move the `consoleprint` function into the `Console` class
public:
	Console& print(const char& str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept
	{
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursorCoordinate();
	}
	Console& println(const char& str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept
	{
		std::print("\033[{};{}m{}\033[{}m\n", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursorCoordinate();
	}
	Console& centered(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept
	{
		short padding = (width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		if (newline)std::print("\n");
		return this->getCursorCoordinate();
	}
	Console& centered(const std::string_view str, const bool& newline) noexcept
	{
		return this->centered(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept
	{
		short padding = width - str.length();
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		if (newline)std::print("\n");
		return this->getCursorCoordinate();
	}
	Console& rightAligned(const std::string_view str, const bool& newline) noexcept
	{
		return this->rightAligned(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept
	{
		this->getCursorCoordinate();
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		if (newline)std::print("\n");
		return this->getCursorCoordinate();
	}
	Console& setStyle(const ansiStyle& style = ansiStyle::reset) noexcept
	{
		std::print("\033[{}m",
			static_cast<int>(style));
		return this->getCursorCoordinate();
	}
	Console& fillLine(const char& fillChar = ' ', const int& dRow = 1, const bool& newline = true)noexcept
	{
		if (dRow < 1)
			return *this;
		this->setCursorCoordinate(cursorRow - 1, 0);
		std::string line(width - 1, fillChar);
		for (int count = 0; count < dRow; count++) {
			std::println("\033[{}m{}\033[{}m", static_cast<int>(ansiStyle::reset), line, static_cast<int>(ansiStyle::reset));
		}
		return this->getCursorCoordinate();
	}
public:
	//decltype(&Console::print) log = &this->print;
}console;

// failed to compile	
//// variadic template, perfect forwarding
//template<typename... Args>
//Console& print(const ansiColor& color, const ansiBackground& background, Args&&... args) {
//	std::print("\033[{};{}m", static_cast<int>(background), static_cast<int>(color));
//	std::print(std::forward<Args>(args)...);
//	std::print("\033[{}m", static_cast<int>(ansiStyle::reset));
//	return this->getCursorCoordinate();