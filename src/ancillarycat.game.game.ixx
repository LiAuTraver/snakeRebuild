module;
#include "../include/config.hpp"
export module ancillarycat.game:game;
import :leaderboard;
import ancillarycat.utils;
import ancillarycat.console;
import ancillarycat.api;
import ancillarycat.entities;

namespace game
{
void score_logger(const short& score)
{
	const std::string current_score = "Score: " + std::to_string(score);
	const short len = static_cast<short>((INFO_WIDTH - current_score.length()) / 2 + INFO_COL);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(INFO_ROW + 1, len, current_score, ansiColor::redIntense);
}
void time_logger(const std::chrono::seconds& cur, const short& row, const short& col) {
	const std::string elapsed = "Time: " + std::to_string(cur.count()) + "s";
	const short len = static_cast<short>((INFO_WIDTH - elapsed.length()) / 2 + col);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(row, len, elapsed, ansiColor::white);
}
int gameOver(Snake& snake) {
	api::soundEvent(LR"(\Media\Windows Critical Stop.wav)", soundFlag::async, soundFlag::async);
	console
		.centeredShuttle(console.height - 2, "Game Over!", ansiColor::red, ansiBackground::black)
		.bot("Press Enter to return to menu", ansiColor::white, ansiBackground::black)
		.setCursorCoordinate(0, 0);
	game::writeBuffer(snake.score, utils::elapsed);
	std::cin.get();
	console.clear();
	return GAMEOVER;
}
void gameInit() {
	console
		.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH)
		.box(INFO_ROW, INFO_COL, INFO_HEIGHT, INFO_WIDTH);
	api::soundEvent(LR"(\Media\Ring01.wav)");
	for (int i = 0; i < 3; i++) {
		console.bot("Game will start in " + std::to_string(3 - i) + " seconds", ansiColor::green, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	console
		.setStyle(ansiStyle::blink)
		.bot("Game Start!", ansiColor::green, ansiBackground::black, true);
}
int snakeGame() {
	console.clear();
	Snake snake('@');
	Food food('$');
	game::gameInit();
	bool isIgnore = false;
	int prevKey = -2;
	while (true) {
		if (console.getch(true)) {
			const int ch = console.getch();
			// alpha 224
			if (ch == 224 || ch == prevKey) continue;
			prevKey = ch;
			switch (ch) {
			case KEY_UP:
				if (snake.nDirection == direction::DOWN || snake.nDirection == direction::UP) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::UP;
				break;
			case KEY_DOWN:
				if (snake.nDirection == direction::UP || snake.nDirection == direction::DOWN) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::DOWN;
				break;
			case KEY_LEFT:
				if (snake.nDirection == direction::RIGHT || snake.nDirection == direction::LEFT) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::LEFT;
				break;
			case KEY_RIGHT:
				if (snake.nDirection == direction::LEFT || snake.nDirection == direction::RIGHT) {
					// ignore the key
					isIgnore = true;
					break;
				}
				snake.nDirection = direction::RIGHT;
				break;
			default:
				isIgnore = true;
				break;
			}
		}
		snake.move(1, 'o');
		if (utils::checkOutofBound(snake) == INVALID) {
			return gameOver(snake);
		}
		if (utils::checkInvalidPosition(snake, food) == INVALID) {
			food.regenerate().show();
			if (utils::checkInvalidPosition(snake.grow(), snake.nodes.back()) == INVALID) {
				utils::elapsed = std::chrono::milliseconds(0);
				return gameOver(snake);
			}
			++snake.score;
			api::soundEvent(LR"(\Media\Windows Proximity Notification.wav)");
		}
		if (snake.check() == INVALID)
		{
			return gameOver(snake);
		}
		if (isIgnore) {
			isIgnore = false;
		}
		game::time_logger(utils::timer(std::chrono::milliseconds(250)), INFO_ROW, INFO_COL);
		game::score_logger(snake.score);
	}
	//std::unreachable();
}
}
