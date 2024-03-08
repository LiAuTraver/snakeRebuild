module;

#include <Windows.h>

#pragma warning(disable:4267)
export module ancillarycat.print;

import std.compat;
import ancillarycat.console;
import ancillarycat.ansi;

export class ConsolePrint
{
public:
	inline ConsolePrint& operator<<(const std::string_view str)
	{
		std::print("{}", str);
		return *this;
	}
	inline ConsolePrint& centered(const std::string_view str)
	{
		short padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::print("{}", str);
		return *this;
	}
	inline ConsolePrint& rightAligned(const std::string_view str)
	{
		short padding = console.width - str.length();
		padding < 0 ? padding = 0 : padding;
		this->setCursor(console.cursorRow, padding);
		std::print("{}", str);
		return *this;
	}
	inline ConsolePrint& leftAligned(const std::string_view str)
	{
		this->setCursor(console.cursorRow, 0);
		std::print("{}", str);
		return *this;
	}
	// does not contain a newline character
	inline ConsolePrint& reset(
		const ansiColor& color = ansiColor::white,
		const ansiBackground& background = ansiBackground::black,
		const ansiStyle& style = ansiStyle::reset) {
		std::print("\033[{};{};{}m",
			static_cast<int>(color),
			static_cast<int>(background),
			static_cast<int>(style));
		return *this;
	}
	inline ConsolePrint& setFontColor(const ansiColor& color)
	{
		std::print("\033[{}m", static_cast<int>(color));
		return *this;
	}
	inline ConsolePrint& setCursor(const SHORT row, const SHORT col)
	{
		console
			.getCursorCoordinate()
			.setCursorCoordinate(row, col);
		return  *this;
	}
	// add/subtract coordinate
	inline ConsolePrint& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0)
	{
		console
			.getCursorCoordinate()
			.moveCursor(deltaRow, deltaCol);
		return  *this;
	}
}consolePrint;

export class ConsolePrintln {
public:
	inline ConsolePrintln& operator<<(const std::string_view str) {
		std::println("{}", str);
		return *this;
	}
	inline ConsolePrintln& centered(const std::string_view str) {
		short padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::println("{}", str);
		return *this;
	}
	inline ConsolePrintln& rightAligned(const std::string_view str) {
		short padding = console.width - str.length();
		padding < 0 ? padding = 0 : padding;
		this->updateCursor();
		std::println("{}", str);
		return *this;
	}
	inline ConsolePrintln& leftAligned(const std::string_view str) {
		this->updateCursor();
		std::println("{}", str);
		return *this;
	}
	inline ConsolePrintln& fillLine(const char fillChar = ' ')
	{
		std::string line(console.width - 1, fillChar);
		std::println("{}", line);
		// have problem
		/*this
			->moveCursor(console.cursorRow + 1, 0)
			.updateCursor();*/
		return *this;
	}
	// does not contain a newline character
	inline ConsolePrintln& reset(
		const ansiColor& color = ansiColor::white,
		const ansiBackground& background = ansiBackground::black,
		const ansiStyle& style = ansiStyle::reset) {
		std::println("\033[{};{};{}m",
			static_cast<int>(color),
			static_cast<int>(background),
			static_cast<int>(style));
		return *this;
	}
	inline ConsolePrintln& setFontColor(const ansiColor& color)
	{
		std::println("\033[{}m", static_cast<int>(color));
		return *this;
	}

	inline ConsolePrintln& updateCursor() {
		console
			.getCursorCoordinate()
			.setCursorCoordinate();
		return  *this;
	}
	// add/subtract coordinate
	inline ConsolePrintln& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0) {
		console
			.getCursorCoordinate()
			.moveCursor(deltaRow, deltaCol);
		return  *this;
	}
}consolePrintln;

#pragma warning(default:4267)
