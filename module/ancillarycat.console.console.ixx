module;
#include "../include/config.hpp"
#include <Windows.h>
export module ancillarycat.console:console;
import std;
import ancillarycat.api;

export class Console;

class Console final {
public:
	explicit Console();
	Console& oninitialize() noexcept;
	Console& setCursorState(const bool& state = false) noexcept;
	Console& getCursorCoordinate() noexcept;
	Console& setCursorCoordinate(const SHORT row = DEFAULT_VAL, const SHORT col = DEFAULT_VAL){
		return this->set_cur_coor_impl(row, col).getCursorCoordinate();
	}
	Console& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0);
	Console& resetAnsi() noexcept;
	Console& setAnsi(const WORD attribute) noexcept;
	static int getch(const bool& notNeedRealKey = false) noexcept;
	Console& box(const SHORT& curRow = 0, const SHORT& curCol = 0, SHORT boxHeight = DEFAULT_VAL, SHORT boxWidth = DEFAULT_VAL, const char& border = '+', const char& horizontal = '-', const char& vertical = '|');
    template<typename... Args>
    Console& print(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, Args&&... args) noexcept
    {
        std::print("\033[{};{}m{}\033[{}m", static_cast<int>(background), static_cast<int>(color), std::vformat(str, std::make_format_args(std::forward<Args>(args)...)), static_cast<int>(ansiStyle::reset));
        return this->getCursorCoordinate();
    }
    template <typename... Args>
    Console& println(const std::string& str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, Args&&... args) noexcept
    {
        return this->print(str + '\n', color, background, std::forward<Args>(args)...)
                    .getCursorCoordinate();
    }
    template <typename... Args>
    Console& log(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, Args&&... args) noexcept
    {
        return this->print(str, color, background, std::forward<Args>(args)...);
    }
    template <typename... Args>
    Console &centered(const std::string_view str, const ansiColor &color = ansiColor::white, const ansiBackground &background = ansiBackground::reset, const bool &newline = true, Args&&... args) noexcept {
        auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
        short padding = static_cast<short>((width - formattedString.length()) / 2);
        padding = std::max<short>(padding, 0);

        std::cout << "\r\033[K"; // Clear the line
        if (newline)
            return this->moveCursor(0, padding)
            .println("{}", color, background, formattedString)
            .getCursorCoordinate();
        else
            return this->moveCursor(0, padding)
            .print("{}", color, background, formattedString)
            .getCursorCoordinate();
    }
    Console &centered(const std::string_view str, const bool& newline) noexcept {
        return this->centered(str, ansiColor::white, ansiBackground::reset, newline);
    }
    template <typename... Args>
    Console &leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool &newline = true, Args&&...args) noexcept {
        auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
        // use '\r', do not clear the line
        if (newline)
            return this->print("\r{}", color, background, formattedString)
            .getCursorCoordinate();
        else
            return this->print("\r{}", color, background, formattedString)
            .getCursorCoordinate();
    }
    Console &leftAligned(const std::string_view str, const bool& newline) noexcept {
        return this->leftAligned(str, ansiColor::white, ansiBackground::reset, newline);
    }
    template <typename... Args>
    Console &rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool &newline = true, Args&&...args) noexcept {
        auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
        short padding = static_cast<short>(width - formattedString.length());
        padding = std::max<short>(padding, 0);
        if(newline)
            return this->moveCursor(0, padding)
            .println("{}", color, background, formattedString)
            .getCursorCoordinate();
        else
            return this->moveCursor(0, padding)
            .print("{}", color, background, formattedString)
            .getCursorCoordinate();
    }
    Console &rightAligned(const std::string_view str, const bool& newline) noexcept {
        return this->rightAligned(str, ansiColor::white, ansiBackground::reset, newline);
    }
	Console& setStyle(const ansiStyle& style = ansiStyle::reset) noexcept;
	Console& fillLine(const char& fillChar = ' ', const int& dRow = 1, const bool& newline = false)noexcept;
	Console& centeredShuttle(const SHORT& lineNum, const std::string_view str, const ansiColor& color, const ansiBackground& background,const bool& needsRevisited = false) noexcept
	{
		return this->return_impl_ln(lineNum, str, color, background, needsRevisited);
	}
	Console& shuttle(const SHORT& row, const SHORT& col, const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& needsRevisited = false) noexcept
	{
		return this->return_impl(row, col, str, color, background, needsRevisited);
	}
	Console& top(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& needsRevisited = false) noexcept {
		return this->centeredShuttle(0, str, color, background,needsRevisited);
	}
	Console& bot(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& needsRevisited = false) noexcept {
		return this->centeredShuttle(height - 1, str, color, background,needsRevisited);
	}
	Console& set_cur_coor_impl(const SHORT& row, const SHORT& col) noexcept;
	Console& clear() noexcept;

private:
	std::mutex cursorMutex_;
	Console& return_impl(const SHORT& row, const SHORT& col, const std::string_view str, const ansiColor& color, const ansiBackground& background, const bool& needsRevisited = false) noexcept;
	Console& return_impl_ln(const SHORT& row, const std::string_view str, const ansiColor& color, const ansiBackground& background, const bool& needsRevisited = false) noexcept;
	Console& printM(const std::string_view str, const short& prevY,const short& prevX,const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset) noexcept;
public:
	SHORT width;
	SHORT height;
	COORD cursorCoordinate;
	SHORT& cursorRow = cursorCoordinate.Y;
	SHORT& cursorCol = cursorCoordinate.X;
private:
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo_{};
	HANDLE handleStdin_{};
	HANDLE handleStdout_{};
	DWORD stdinMode_{};
	DWORD stdoutMode_{};
	CONSOLE_CURSOR_INFO cursorInfo_{};
};

export Console console;
