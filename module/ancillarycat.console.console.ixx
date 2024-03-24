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
	Console& reset(const bool& visibility = false) noexcept;
	Console& setCursorState(const bool& state = false) noexcept;
	Console& getCursorCoordinate() noexcept;
	Console& setCursorCoordinate(const SHORT row = DEFAULT_VAL, const SHORT col = DEFAULT_VAL) {
		return this->set_cur_coor_impl(row, col)
			.getCursorCoordinate();
	}
	Console& moveCursor(const SHORT deltaRow = 0, const SHORT deltaCol = 0);
	Console& resetAnsi() noexcept;
	Console& setAnsi(const WORD attribute) noexcept;
	static int getch(const bool& notNeedRealKey = false) noexcept;
	Console& box(const SHORT& curRow = 0, const SHORT& curCol = 0, SHORT boxHeight = DEFAULT_VAL, SHORT boxWidth = DEFAULT_VAL, const char& border = '+', const char& horizontal = '-', const char& vertical = '|');
	template<typename... Args>
	Console& print(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, Args&&... args) noexcept
	{
		std::print("\033[{};{}m{}", static_cast<int>(background), static_cast<int>(color), std::vformat(str, std::make_format_args(std::forward<Args>(args)...)));
		std::print("\033[{}m", static_cast<int>(ansiStyle::reset));
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
	Console& centered(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true, Args&&... args) noexcept {
		auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
		short padding = std::max<short>(static_cast<short>((width - formattedString.length()) / 2), 0);
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
	Console& centered(const std::string_view str, const bool& newline = true) noexcept {
		return this->centered(str, ansiColor::white, ansiBackground::reset, newline);
	}
	template <typename... Args>
	Console& leftAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true, Args&&...args) noexcept {
		auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
		// use '\r', do not clear the line
		if (newline)
			return this->print("\r{}", color, background, formattedString)
			.getCursorCoordinate();
		else
			return this->print("\r{}", color, background, formattedString)
			.getCursorCoordinate();
	}
	Console& leftAligned(const std::string_view str, const bool& newline) noexcept {
		return this->leftAligned(str, ansiColor::white, ansiBackground::reset, newline);
	}
	template <typename... Args>
	Console& rightAligned(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const bool& newline = true, Args&&...args) noexcept {
		auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
		if (newline)
			return this->moveCursor(0, std::max<short>(static_cast<short>(width - formattedString.length()), 0))
			.println("{}", color, background, formattedString)
			.getCursorCoordinate();
		else
			return this->moveCursor(0, std::max<short>(static_cast<short>(width - formattedString.length()), 0))
			.print("{}", color, background, formattedString)
			.getCursorCoordinate();
	}
	template <typename... Args>
	Console& rightAligned(const std::string_view str, const bool& newline) noexcept {
		return this->rightAligned(str, ansiColor::white, ansiBackground::reset, newline);
	}
	Console& setStyle(const ansiStyle& style = ansiStyle::reset) noexcept;
	Console& fillLine(const char& fillChar = ' ', const int& dRow = 1, const bool& newline = false)noexcept;
	template <typename... Args>
	Console& centeredShuttle(const SHORT& lineNum, const std::string_view str, const ansiColor& color, const ansiBackground& background, const unsigned long long& needsRevisited = 0, Args&&... args) noexcept {
		auto formattedString = std::vformat(str, std::make_format_args(std::forward<Args>(args)...));
		return this->print_shuttle_impl(formattedString, cursorRow, cursorCol, lineNum, std::max<short>(static_cast<short>((width - formattedString.length()) / 2), 0), color, background, needsRevisited);
	}
	Console& shuttle(const SHORT& row, const SHORT& col, const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const unsigned long long& needsRevisited = 0) noexcept
	{
		return this->print_shuttle_impl(str, cursorRow, cursorCol, row, col, color, background, needsRevisited);
	}
	Console& top(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const unsigned long long& needsRevisited = 0) noexcept {
		return this->centeredShuttle(0, str, color, background, needsRevisited);
	}
	Console& bot(const std::string_view str, const ansiColor& color = ansiColor::white, const ansiBackground& background = ansiBackground::reset, const unsigned long long& needsRevisited = 0) noexcept {
		return this->centeredShuttle(height - 2, str, color, background, needsRevisited);
	}
	Console& clear() noexcept;

	Console& set_cur_coor_impl(const SHORT& row, const SHORT& col) noexcept;
private:
template <typename... Args>
Console& print_shuttle_impl(const std::string_view str, const short& prevY, const short& prevX, const short& newY, const short& newX, const ansiColor& color, const ansiBackground& background, const unsigned long long& needsRevisited = 3000, Args&&... args) noexcept {
    // Directly modify the cursor position and print
    this->set_cur_coor_impl(newY, newX)
        .print(str, color, background, std::forward<Args>(args)...)
		.set_cur_coor_impl(prevY, prevX);

    // Create a thread to erase the message after 3 seconds
    if (needsRevisited) {
        std::thread([=, this]() {
            // needsRevisited represents milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(needsRevisited));
            std::unique_lock<std::shared_mutex> lock(cursorMutex_);
            this->getCursorCoordinate(); // Get the current cursor position
            auto nowY = cursorRow;
            auto nowX = cursorCol;
            this->set_cur_coor_impl(newY, newX) // Move back to the message's position
                .print(std::string(str.size(), ' ')) // Erase by overwriting with spaces
                .set_cur_coor_impl(nowY, nowX); // Restore the cursor to its previous position
            lock.unlock();
        }).detach(); // Detach the thread to allow it to run independently
    }
    return *this;
}
public:
	SHORT width;
	SHORT height;
	COORD cursorCoordinate;
	SHORT& cursorRow = cursorCoordinate.Y;
	SHORT& cursorCol = cursorCoordinate.X;
private:
	std::shared_mutex cursorMutex_;
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo_{};
	HANDLE handleStdin_{};
	HANDLE handleStdout_{};
	DWORD stdinMode_{};
	DWORD stdoutMode_{};
	CONSOLE_CURSOR_INFO cursorInfo_{};
};

export Console console;
