module;
#include "config.hpp"
#include <Windows.h>
export module ancillarycat.game;
import std;
import ancillarycat.ansi;
import ancillarycat.config;
import ancillarycat.console;
import ancillarycat.entities;
import ancillarycat.leaderboard;
import ancillarycat.windows.api;
import ancillarycat.utils;

export namespace game
{
void time_logger(const std::chrono::seconds&, const int&, const int&);
int gameOver();
void gameInit();
int snakeGame();
void score_logger(const short&);
void time_logger(const std::chrono::seconds& cur, const int& row = INFO_ROW, const int& col = INFO_COL);
}