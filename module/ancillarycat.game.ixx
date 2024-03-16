module;
#include "../include/config.hpp"
export module ancillarycat.game;
import std;
import ancillarycat.api;
import ancillarycat.config;
import ancillarycat.console;
import ancillarycat.entities;
import ancillarycat.leaderboard;
import ancillarycat.utils;

export namespace game
{
int gameOver();
void gameInit();
int snakeGame();
void score_logger(const short&);
void time_logger(const std::chrono::seconds& cur, const short& row = INFO_ROW, const short& col = INFO_COL);
}