#pragma once

// Constants that CANNOT be changed to another value:
constexpr inline int EXIT_PROGRAM = 0;
constexpr inline int RESTART_PROGRAM = 1;
constexpr inline int CONTINUE_PROGRAM = 2;
constexpr inline int UNKNOWN_ERROR = static_cast<int>(static_cast<unsigned>(-1));

constexpr inline short DEFAULT_VAL = -1;
constexpr inline int VALID = 0;
constexpr inline int INVALID = 1;

// Constants that can be changed to another value:
// I am going to use bitwise OR/AND rewrite the error code.
constexpr inline int GENERATE = 1 << 6;
constexpr inline int GENERATE_COOR = 1 << 7;
constexpr inline int GENERATE_DIR = 1 << 8;
constexpr inline int IN_BOUND = 1 << 3;
constexpr inline int OUT_OF_BOUND = 1 <<4;
constexpr inline int INVALID_DIRECTION_INPUT = 1 << 5;

constexpr inline short MIN_HEIGHT = 20; // console window minimum height
constexpr inline short MIN_WIDTH = 40; // console window minimum width
constexpr inline short START_ROW = 5; // game box start row
constexpr inline short START_COL = 5; // game box start column
constexpr inline short BOX_HEIGHT = 20; // game box height
constexpr inline short BOX_WIDTH = 40; // game box width