module;

export module ancillarycat.ansi;

export enum class ansiColor : int {
	black = 30,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

export enum class ansiBackground : int {
	black = 40,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

export enum class ansiHighlight : int {
	black = 90,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

export enum class ansiHighlightBackground : int {
	black = 100,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

export enum class ansiStyle : int {
	reset = 0,
	bold = 1,
	underline = 4,
	blink = 5,
	reverse = 7,
	invisible = 8
};
