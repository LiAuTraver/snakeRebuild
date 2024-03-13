module;
import "config.hpp";
import <Windows.h>;
import <conio.h>;
import ancillarycat.ansi;
import ancillarycat.console;
import std;
export module ancillarycat.impl.console;


inline Console::Console(const SHORT& minWidth, const SHORT& minHeight) : cursorRow(0), cursorCol(0), cursorCoordinate({ 0,0 })
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
inline Console& Console::setCursorState(const bool& state) noexcept
{
	cursorInfo_.bVisible = state;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo_);
	return *this;
}
inline Console& Console::getCursorCoordinate()
{
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
	cursorCoordinate = consoleScreenBufferInfo_.dwCursorPosition;
	cursorRow = cursorCoordinate.Y;
	cursorCol = cursorCoordinate.X;
	return *this;
}
inline Console& Console::setCursorCoordinate(const SHORT row, const SHORT col)
{
	this->getCursorCoordinate();
	cursorCoordinate.X = col;
	cursorCoordinate.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
	return this->getCursorCoordinate();
}
inline Console& Console::moveCursor(const SHORT deltaRow, const SHORT deltaCol)
{
	cursorCoordinate.Y += deltaRow;
	cursorCoordinate.X += deltaCol;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorCoordinate);
	return this->getCursorCoordinate();
}
// output attributes control
inline Console& Console::resetAnsi() noexcept
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return *this;
}
inline Console& Console::setAnsi(const WORD attribute) noexcept
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute);
	return *this;
}
// console interaction
inline const int Console::getch() noexcept {
	return _getch();
}
inline Console& Console::box(const SHORT& curRow, const SHORT& curCol, SHORT boxHeight, SHORT boxWidth, const char& border, const char& horizontal, const char& vertical)
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
// use when the program `restarts` or `resumes` from a `paused` state
inline Console& Console::terminalSizeChange() {
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo_);
	width = consoleScreenBufferInfo_.srWindow.Right - consoleScreenBufferInfo_.srWindow.Left + 1;
	height = consoleScreenBufferInfo_.srWindow.Bottom - consoleScreenBufferInfo_.srWindow.Top + 1;
	return *this;
}

inline Console& Console::print(const char& str, const ansiColor& color, const ansiBackground& background) noexcept
{
	std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
	return this->getCursorCoordinate();
}
inline Console& Console::println(const char& str, const ansiColor& color, const ansiBackground& background) noexcept
{
	std::print("\033[{};{}m{}\033[{}m\n", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
	return this->getCursorCoordinate();
}
inline Console& Console::centered(const std::string_view str, const ansiColor& color, const ansiBackground& background, const bool& newline) noexcept
{
	short padding = (width - str.length()) / 2;
	padding < 0 ? padding = 0 : padding;
	this->moveCursor(0, padding);
	std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
	if (newline)std::print("\n");
	return this->getCursorCoordinate();
}

inline Console& Console::rightAligned(const std::string_view str, const ansiColor& color, const ansiBackground& background, const bool& newline) noexcept
{
	short padding = width - str.length();
	padding < 0 ? padding = 0 : padding;
	this->moveCursor(0, padding);
	std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
	if (newline)std::print("\n");
	return this->getCursorCoordinate();
}

inline Console& Console::leftAligned(const std::string_view str, const ansiColor& color, const ansiBackground& background, const bool& newline) noexcept
{
	this->getCursorCoordinate();
	std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
	if (newline)std::print("\n");
	return this->getCursorCoordinate();
}
inline Console& Console::setStyle(const ansiStyle& style) noexcept
{
	std::print("\033[{}m",
		static_cast<int>(style));
	return this->getCursorCoordinate();
}
inline Console& Console::fillLine(const char& fillChar, const int& dRow, const bool& )noexcept
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

//decltype(&Console::print) log = &this->print;

// failed to compile	
//// variadic template, perfect forwarding
//template<typename... Args>
//Console& Console::print(const ansiColor& color, const ansiBackground& background, Args&&... args) {
//	std::print("\033[{};{}m", static_cast<int>(background), static_cast<int>(color));
//	std::print(std::forward<Args>(args)...);
//	std::print("\033[{}m", static_cast<int>(ansiStyle::reset));
//	return this->getCursorCoordinate();