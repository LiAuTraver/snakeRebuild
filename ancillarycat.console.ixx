module;

#include <Windows.h>
import std;

export module ancillarycat.console;


export class Console {
public:
    Console() {
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
        width = csbiInfo.srWindow.Right - csbiInfo.srWindow.Left + 1;
        height = csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1;
        if(width <= 0 || height <= 0)
        {
	        			throw std::runtime_error("Console width and height must be greater than 0");
		}
    }
    Console(const SHORT minWidth, const SHORT minHeight)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
        width = csbiInfo.srWindow.Right - csbiInfo.srWindow.Left + 1;
        height = csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1;
        if (width < minWidth || height <= minHeight)
        {
            throw std::runtime_error("Console width and height must be not less than the given minimum width and height");
        }
	}
public:
    SHORT width;
    SHORT height;
}console;