module;
export module ancillarycat.api:ansi;
import std;

export enum class ansiColor : int {
	reset = 0,
	black = 30,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,
	blackIntense = 90,
	redIntense,
	greenIntense,
	yellowIntense,
	blueIntense,
	magentaIntense,
	cyanIntense,
	whiteIntense
};

export enum class ansiBackground : int {
	reset = 0,
	black = 40,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,
	blackIntense = 100,
	resIntense,
	greenIntense,
	yellowIntense,
	blueIntense,
	magentaIntense,
	cyanIntense,
	whiteIntense
};


export enum class ansiStyle : int {
	reset = 0,
	bold = 1, // does not work in my terminal
	underline = 4,
	blink = 5, // text blinking
	reverse = 7, // reverse the color of the text and background
	invisible = 8 // text invisible
};
export const std::array<std::string, 5> SNAKE{
R"( ____              _        )",
R"(/ ___| _ __   __ _| | _____ )",
R"(\___ \| '_ \ / _` | |/ / _ \)",
R"( ___) | | | | (_| |   <  __/)",
R"(|____/|_| |_|\__,_|_|\_\___|)"
};
export const std::array<std::string, 5> SNAKE_GAME{
R"( ____              _           ____                      )",
R"(/ ___| _ __   __ _| | _____   / ___| __ _ _ __ ___   ___ )",
R"(\___ \| '_ \ / _` | |/ / _ \ | |  _ / _` | '_ ` _ \ / _ \)",
R"( ___) | | | | (_| |   <  __/ | |_| | (_| | | | | | |  __/)",
R"(|____/|_| |_|\__,_|_|\_\___|  \____|\__,_|_| |_| |_|\___|)"
};
export const std::array<std::string, 5> LEAD{
	R"( _     _____    _    ____  )",
	R"(| |   | ____|  / \  |  _ \ )",
	R"(| |   |  _|   / _ \ | | | |)",
	R"(| |___| |___ / ___ \| |_| |)",
	R"(|_____|_____/_/   \_\____/ )"
};
export const std::array<std::string, 5> TOP10{
	R"( _____ ___  ____    _  ___  )",
	R"(|_   _/ _ \|  _ \  / |/ _ \ )",
	R"(  | || | | | |_) | | | | | |)",
	R"(  | || |_| |  __/  | | |_| |)",
	R"(  |_| \___/|_|     |_|\___/ )"
};

export const std::array<std::string, 6> CONFIG{
	R"(  ____             __ _       )",
	R"( / ___|___  _ __  / _(_) __ _ )",
	R"(| |   / _ \| '_ \| |_| |/ _` |)",
	R"(| |__| (_) | | | |  _| | (_| |)",
	R"( \____\___/|_| |_|_| |_|\__, |)",
	R"(                        |___/ )"
};

