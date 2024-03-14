module;
#include "config.hpp"
export module ancillarycat.game;
import <Windows.h>;
import std;
import ancillarycat.ansi;
import ancillarycat.config;
import ancillarycat.console;
import ancillarycat.snake;
import ancillarycat.leaderboard;
import ancillarycat.windows.api;
import ancillarycat.entity;
namespace game
{
export void clock(const std::chrono::seconds&, const int&, const int&);
export int gameOver();
export void gameInit();
export int snakeGame();
}
namespace game
{
export DEFINITION void clock(const std::chrono::seconds& cur, const int& row = TIMER_ROW, const int& col = TIMER_COL) {
	console
		.printAndReturn(TIMER_ROW, TIMER_COL, std::to_string(cur.count()));
}
export DEFINITION int gameOver() {
	api::soundEvent(L"\\Media\\Windows Critical Stop.wav");
	console
		.bot("Game Over!", ansiColor::red, ansiBackground::black)
		.centeredAndReturn(console.height - 2, "Press Enter to return to menu", ansiColor::white, ansiBackground::black);
	game::elapsed = std::chrono::milliseconds(0);
	console.setCursorCoordinate(0, 0);
	std::cin.get();
	system("CLS");
	return GAMEOVER;
}
export DEFINITION void gameInit() {
	system("CLS");
	console.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH)
		.box(TIMER_ROW, TIMER_COL, TIMER_HEIGHT, TIMER_WIDTH);
	api::soundEvent(L"\\Media\\Ring01.wav");
	for (int i = 0; i < 3; i++) {
		console.bot("Game will start in " + std::to_string(3 - i) + " seconds", ansiColor::green, ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	console.bot("Game Start!", ansiColor::green, ansiBackground::black);
}
DEFINITION int snakeGame() {
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
		snake.move();
		food.show();
		if (checkOutofBound(snake) == INVALID) {
			return gameOver();
		}
		if (checkInvalidPosition(snake, food) == INVALID)
		{
			food.regenerate().show();
			// TODO: implement the `grow` function
			if (snake.grow() == INVALID)
			{
				return gameOver();
			}
			api::soundEvent(L"\\Media\\Windows Proximity Notification.wav");
		}
		if (snake.check() == INVALID)
		{
			return gameOver();
		}
		if (isIgnore) {
			isIgnore = false;
		}
		else
		{
			game::timer();
		}
	}
	//std::unreachable();
}
}