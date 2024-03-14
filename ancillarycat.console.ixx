module;
#include "config.hpp";
export module ancillarycat.console;
import <windows.h>;
import ancillarycat.ansi;
import std;

export class Console final {
public:
	explicit Console();
	Console& setCursorState(const bool& state = true) noexcept;
	Console& getCursorCoordinate() noexcept;
	Console& setCursorCoordinate(const SHORT row = DEFAULT_VAL, const SHORT col = DEFAULT_VAL);
	Console& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0);
	Console& resetAnsi() noexcept;
	Console& setAnsi(const WORD attribute) noexcept;
	static int getch(const bool& needRealKey = false) noexcept;
	Console& box(const SHORT& curRow = 0, const SHORT& curCol = 0, SHORT boxHeight = DEFAULT_VAL, SHORT boxWidth = DEFAULT_VAL, const char& border = '+', const char& horizontal = '-', const char& vertical = '|');
	Console& terminalSizeChange();
	Console& print(std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept;
	Console& println(std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept;
	Console& centered(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept;
	Console& centered(const std::string_view str, const bool& newline) noexcept {
		return this->centered(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept;
	Console& leftAligned(const std::string_view str, const bool& newline) noexcept {
		return this->leftAligned(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true) noexcept;
	Console& rightAligned(const std::string_view str, const bool& newline) noexcept {
		return this->rightAligned(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& setStyle(const ansiStyle& style = ansiStyle::reset) noexcept;
	Console& fillLine(const char& fillChar = ' ', const int& dRow = 1, const bool& newline = false)noexcept;
	Console& centeredAndReturn(const SHORT& lineNum, const std::string_view str, const ansiColor& color, const ansiBackground& background) noexcept
	{
		return this->return_impl_ln(lineNum, str, color, background);
	}
	Console& printAndReturn(const SHORT& row, const SHORT& col, const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept
	{
		return this->return_impl(row, col, str, color, background);
	}
	Console& top(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept {
		return this->centeredAndReturn(0, str, color, background);
	}
	Console& bot(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept {
		return this->centeredAndReturn(height - 1, str, color, background);
	}

private:
	Console& return_impl(const SHORT& row, const SHORT& col, const std::string_view str, const ansiColor& color, const ansiBackground& background) noexcept;
	Console& return_impl_ln(const SHORT& row, const std::string_view str, const ansiColor& color, const ansiBackground& background) noexcept;
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
}console;