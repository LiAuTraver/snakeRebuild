module;
#include "../include/config.hpp"
export module ancillarycat.game;
import :leaderboard;
import :game;
import :config;
import std;
export namespace game
{
int gameOver(Snake&);
void gameInit();
int snakeGame();
void score_logger(const short&);
void time_logger(const std::chrono::seconds& cur, const short& row = INFO_ROW, const short& col = INFO_COL);
int leaderboardInit();
int writeBuffer(const short& score, const std::chrono::milliseconds& gameTime);
int snakeLeaderboard();
void config();
int readLeaderboard();
}