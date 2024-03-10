#pragma once

constexpr const inline int EXIT_PROGRAM = 0;
constexpr const inline int RESTART_PROGRAM = 1;
constexpr const inline int CONTINUE_PROGRAM = 2;
constexpr const inline int UNKNOWN_ERROR = static_cast<int>(static_cast<unsigned>(-1));

constexpr const inline short DEFAULT_VAL = -1;
constexpr const inline int VALID = 0;
constexpr const inline int INVALID = 1;

constexpr const inline short MIN_HEIGHT = 20; // console window minimum height
constexpr const inline short MIN_WIDTH = 40; // console window minimum width
constexpr const inline short START_ROW = 5; // game box start row
constexpr const inline short START_COL = 5; // game box start column
constexpr const inline short BOX_HEIGHT = 20; // game box height
constexpr const inline short BOX_WIDTH = 40; // game box width