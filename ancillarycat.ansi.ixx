module;

export module ancillarycat.ansi;

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
