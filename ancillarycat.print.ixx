module;

#include "config.hpp"
#include <Windows.h>

#pragma warning(disable:4267)
export module ancillarycat.print;

import std.compat;
import ancillarycat.console;
import ancillarycat.ansi;

export class ConsolePrint
{
public:
	inline ConsolePrint& print(const char& str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset)
	{
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrint& centered(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset)
	{
		short padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrint& rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset)
	{
		short padding = console.width - str.length();
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrint& leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset)
	{
		this->getCursor();
		std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrint& fillLine(const char fillChar = ' ', const int dRow = 1) {
		if (dRow < 1)
			return *this;
		console
			.getCursorCoordinate()
			.setCursorCoordinate(console.cursorRow - 1, 0);
		std::string line(console.width - 1, fillChar);
		for (int count = 0; count < 1; count++) {
			std::println("\033[{}m{}\033[{}m", static_cast<int>(ansiStyle::reset), line, static_cast<int>(ansiStyle::reset));
		}
		return this->getCursor();
	}
	inline ConsolePrint& setStyle(const ansiStyle& style = ansiStyle::reset)
	{
		std::print("\033[{}m",
			static_cast<int>(style));
		return this->getCursor();
	}
	inline ConsolePrint& getCursor()
	{
		console
			.getCursorCoordinate();
		return  *this;
	}
	inline ConsolePrint& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0)
	{
		console
			.moveCursor(deltaRow, deltaCol);
		return  this->getCursor();
	}
	inline ConsolePrint& setCursor(const SHORT row = console.cursorRow, const SHORT col = console.cursorCol)
	{
		console
			.setCursorCoordinate(row, col);
		return  this->getCursor();
	}
}consolePrint;

export class ConsolePrintln {
public:
	inline ConsolePrintln& println(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset)
	{
		std::println("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrintln& centered(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) {
		short padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::println("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrintln& rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) {
		short padding = console.width - str.length();
		padding < 0 ? padding = 0 : padding;
		this->moveCursor(0, padding);
		std::println("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrintln& leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) {
		this->getCursor();
		std::println("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), str, static_cast<int>(ansiStyle::reset));
		return this->getCursor();
	}
	inline ConsolePrintln& fillLine(const char fillChar = ' ', const int dRow = 1)
	{
		if (dRow < 1)
			return *this;
		console
			.getCursorCoordinate()
			.setCursorCoordinate(console.cursorRow - 1, 0);
		std::string line(console.width - 1, fillChar);
		for (int count = 0; count < dRow; count++) {
			std::println("\033[{}m{}\033[{}m", static_cast<int>(ansiStyle::reset), line, static_cast<int>(ansiStyle::reset));
		}
		return this->getCursor();
	}
	// does not contain a newline character
	inline ConsolePrintln& setStyle(const ansiStyle& style = ansiStyle::reset) {
		std::print("\033[{}m",
			static_cast<int>(style));
		return this->getCursor();
	}
	inline ConsolePrintln& getCursor() {
		console
			.getCursorCoordinate();
		return  *this;
	}
	// add/subtract coordinate
	inline ConsolePrintln& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0) {
		console
			.moveCursor(deltaRow, deltaCol);
		return  this->getCursor();
	}
	inline ConsolePrintln& setCursor(const SHORT row = console.cursorRow, const SHORT col = console.cursorCol)
	{
		console
			.setCursorCoordinate(row, col);
		return  this->getCursor();
	}
}consolePrintln;

#pragma warning(default:4267)
