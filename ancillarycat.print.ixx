module;

#include <string_view>
#include <Windows.h>

export module ancillarycat.print;
import ancillarycat.console;


export class consolePrint {
public:
	inline consolePrint& operator<<(std::string_view str) {
		printf("%s", str.data());
		return *this;
	}
	inline consolePrint& centered(std::string_view str) {
		int padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		printf("\033[%dG%s", padding, str.data());
		return *this;
	}
	inline consolePrint& rightAligned(std::string_view str) {
		int padding = console.width - str.length() + 1;
		padding < 0 ? padding = 0 : padding;
		printf("\033[%dG%s", padding, str.data());
		return *this;
	}
	inline consolePrint& leftAligned(std::string_view str) {
		printf("\033[%dG%s", 0, str.data());
		return *this;
	}
}consolePrint;

export class consolePrintln {
public:
	inline consolePrintln& operator<<(std::string_view str) {
		printf("%s\n", str.data());
		return *this;
	}
	inline consolePrintln& centered(std::string_view str) {
		int padding = (console.width - str.length()) / 2;
		padding < 0 ? padding = 0 : padding;
		printf("\033[%dG%s\n", padding, str.data());
		return *this;
	}
	inline consolePrintln& rightAligned(std::string_view str) {
		int padding = console.width - str.length() + 1;
		padding < 0 ? padding = 0 : padding;
		printf("\033[%dG%s\n", padding, str.data());
		return *this;
	}
	inline consolePrintln& leftAligned(std::string_view str) {
		printf("\033[%dG%s\n", 0, str.data());
		return *this;
	}
}consolePrintln;
