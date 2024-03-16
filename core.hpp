#pragma once
#pragma warning(disable: 5422)
// Constants that CANNOT be changed to another value:
constexpr inline int EXIT_PROGRAM = 0;
constexpr inline int RESTART_PROGRAM = 1;
constexpr inline int CONTINUE_PROGRAM = 2;
constexpr inline int UNKNOWN_ERROR = static_cast<int>(static_cast<unsigned>(-1));

constexpr inline short DEFAULT_VAL = -1;
constexpr inline int VALID = 0;
constexpr inline int INVALID = 1;

constexpr inline int KEY_UP = 72;
constexpr inline int KEY_DOWN = 80;
constexpr inline int KEY_LEFT = 75;
constexpr inline int KEY_RIGHT = 77;

// Constants that can be changed to another value:
// I am going to use bitwise OR/AND rewrite the error code.
constexpr inline int GENERATE = 1 << 6;
constexpr inline int GENERATE_COOR = 1 << 7;
constexpr inline int GENERATE_DIR = 1 << 8;
constexpr inline int GENERATE_SNAKE = 1 << 9;
constexpr inline int IN_BOUND = 1 << 3;
constexpr inline int OUT_OF_BOUND = 1 <<4;
constexpr inline int INVALID_DIRECTION_INPUT = 1 << 5;
constexpr inline int GAMEOVER = 1 << 10;

constinit inline long long INTERVALS = 250; // game speed, fps = 1000 / INTERVALS
constinit inline short MIN_HEIGHT = 20; // console window minimum height
constinit inline short MIN_WIDTH = 40; // console window minimum width
constinit inline short START_ROW = 5; // game box start row
constinit inline short START_COL = 5; // game box start column
constinit inline short BOX_HEIGHT = 20; // game box height
constinit inline short BOX_WIDTH = 50; // game box width
constinit inline short INFO_ROW = 14; // later specified
constinit inline short INFO_COL = 65;// later specified
constinit inline short INFO_HEIGHT = 3; // timer height, plus the border
constinit inline short INFO_WIDTH = 30; // timer width, plus the border
enum class direction : int {
	NO_DIRECTION = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};