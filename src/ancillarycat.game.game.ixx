module;
#include "../include/config.hpp"
export module ancillarycat.game:game;

import :leaderboard;
import ancillarycat.utils;
import ancillarycat.console;
import ancillarycat.api;
import ancillarycat.entities;
import std;

namespace game {
constinit std::unique_ptr<Snake> snake = nullptr;
std::vector<std::unique_ptr<Food>> foods{};
constinit bool isIgnore = false;
constinit int prevKey = -2;
void score_logger(const short& score) {
	const std::string current_score = "Score: " + std::to_string(score);
	const auto len = static_cast<short>((INFO_WIDTH - current_score.length()) / 2 + INFO_COL);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(INFO_ROW + 1, len, current_score, ansiColor::redIntense, ansiBackground::black);
}
void time_logger(const std::chrono::seconds& cur, const short& row, const short& col) {
	const std::string elapsed = "Time: " + std::to_string(cur.count()) + "s";
	const auto len = static_cast<short>((INFO_WIDTH - elapsed.length()) / 2 + col);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(row, len, elapsed, ansiColor::white, ansiBackground::black);
}
int gameOver() {
	api::soundEvent(LR"(\Media\Windows Critical Stop.wav)", soundFlag::async, soundFlag::async);
	console
		.centeredShuttle(console.height - 2, "Game Over!", ansiColor::red, ansiBackground::black)
		.bot("Press Enter to return to menu", ansiColor::white, ansiBackground::black)
		.setCursorCoordinate(0, 0);
	game::writeBuffer(snake->score, utils::elapsed);
	std::cin.get();
	utils::elapsed = std::chrono::milliseconds(0);
	console.clear();
	foods.clear();
	snake = nullptr;
	return GAMEOVER;
}
void gameInit() {
	isIgnore = false;
	prevKey = -2;
	console
		.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH)
		.box(INFO_ROW, INFO_COL, INFO_HEIGHT, INFO_WIDTH);
	api::soundEvent(LR"(\Media\Ring01.wav)");
	for (int i = 0; i < 3; i++) {
		console.bot("Game will start in " + std::to_string(3 - i) + " seconds", ansiColor::green,
			ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	console
		.setStyle(ansiStyle::blink)
		.bot("Game Start!", ansiColor::green, ansiBackground::black, true);
}
int input_key()
{
	const int ch = console.getch();
	// alpha 224
	if (ch == 224 || ch == prevKey) return CONTINUE_PROGRAM;
	prevKey = ch;
	switch (ch) {
	case 'w':
		[[fallthrough]];
	case 'W':
		[[fallthrough]];
	case KEY_UP:
		if (snake->nDirection == direction::DOWN || snake->nDirection == direction::UP) {
			// ignore the key
			isIgnore = true;
			return 0;
			// std::unreachable();
		}
		snake->nDirection = direction::UP;
		break;
	case 's':
		[[fallthrough]];
	case 'S':
		[[fallthrough]];
	case KEY_DOWN:
		if (snake->nDirection == direction::UP || snake->nDirection == direction::DOWN) {
			// ignore the key
			isIgnore = true;
			return 0;
			// std::unreachable();
		}
		snake->nDirection = direction::DOWN;
		break;
	case 'a':
		[[fallthrough]];
	case 'A':
		[[fallthrough]];
	case KEY_LEFT:
		if (snake->nDirection == direction::RIGHT || snake->nDirection == direction::LEFT) {
			// ignore the key
			isIgnore = true;
			return 0;
			// std::unreachable();
		}
		snake->nDirection = direction::LEFT;
		return 0;
		// std::unreachable();
	case 'd':
		[[fallthrough]];
	case 'D':
		[[fallthrough]];
	case KEY_RIGHT:
		if (snake->nDirection == direction::LEFT || snake->nDirection == direction::RIGHT) {
			// ignore the key
			isIgnore = true;
			return 0;
			// std::unreachable();
		}
		snake->nDirection = direction::RIGHT;
		return 0;
		// std::unreachable();
	default:
		isIgnore = true;
		return 0;
		// std::unreachable();
	}
	// it is reachable? I am confused.
	 return 0;
}
std::unique_ptr<Food> make_valid_food()
{
	auto p = generate_food();
	bool flag = false;
	while (true) {
		if (utils::checkInvalidPosition(*snake, *p) == INVALID) {
			p = generate_food();
			continue;
		}
		for (const auto& node : snake->nodes) {
			if (utils::checkInvalidPosition(*snake, node) == INVALID) {
				p = generate_food();
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			continue;
		}
		for (size_t k = 0; k < foods.size(); k++) {
			if (utils::checkInvalidPosition(*p, *foods.at(k)) == INVALID) {
				p = generate_food();
				flag = true;
				break;
			}
		}
		// TODO: obstacle
		if (!flag) {
			break;
		}
		flag = false;
	}
	return std::move(p);
}
int snakeGame() {
	console.clear();
	snake = std::make_unique<Snake>(Snake('@'));
	foods.emplace_back(make_valid_food())->show();
	std::ranges::for_each(foods, [](const auto& food) { food->show(); });
	game::gameInit();
	while (true) {
		if (console.getch(true) && input_key()) continue;
		if (snake->move(1, 'o').check() == INVALID || utils::checkOutofBound(*snake) == INVALID)return gameOver();
		for (size_t i = 0; i < foods.size(); i++) {
			if (utils::checkInvalidPosition(*snake, *foods.at(i)) == INVALID) {
				snake->score += foods.at(i)->weight();
				if (snake->
					grow(foods.at(i)->weight(), foods.at(i)->weight())
					.show().check() == INVALID) {
					return gameOver();
				}
				foods.erase(foods.begin() + i);
				api::soundEvent(LR"(\Media\Windows Proximity Notification.wav)");
				if (foods.size() <= 7) {
					const int genNum = generator.single(0, 1) || foods.empty() ? generator.single(0, 3) : 0;
					for (size_t j = 0; j < genNum; j++) {
						foods.emplace_back(make_valid_food())->show();
					}
				}
			}
		}
		if (foods.empty())foods.emplace_back(make_valid_food())->show();
		if (isIgnore) isIgnore = false;
		game::time_logger(utils::timer(std::chrono::milliseconds(250)), INFO_ROW, INFO_COL);
		game::score_logger(snake->score);
	}
	//std::unreachable();
}

}

// below was a wrong example
#pragma region WRONG EXAMPLE: editing the vector while iterating
//		for (auto& food : foods) {
//			if (food == nullptr)continue;
//			if (snake->getY() == food->getY() && snake->getX() == food->getX()) {
//				food->regenerate().show();
//				if (foods.size() <= 7) {
//					std::vector<std::unique_ptr<Food>> p;
//					if (generator.single(0, 1)) {
//						for (size_t i = 0; i < generator.single(0, 3); i++) {
//							foods.emplace_back(std::move(generate_food()))->show();
//						}
//					}
//					// add p to foods, need std::move!!! if p is a vector of unique_ptr,
//					// WRONG EXAMPLE: foods.insert(foods.end(), p.begin(), p.end());
//					//std::ranges::move(p, std::back_inserter(foods));
//				}
//				if (utils::checkInvalidPosition(snake->grow(food->weight(), food->weight()).show(), snake->nodes.back()) ==
//					INVALID) {
//					return gameOver(*snake);
//				}
//				snake->score += food->weight();
//				api::soundEvent(LR"(\Media\Windows Proximity Notification.wav)");
//			}
//		}

// rewrite
//        auto it = foods.begin();
//        while(it < foods.end()){
//            if (snake->getY() == (*it)->getY() && snake->getX() == (*it)->getX()) {
//                it->reset();
//                *it = std::move(generate_food());
//                (*it)->show();
//                if (foods.size() <= 7) {
//                    if (generator.single(0, 1)) {
//                        for (size_t i = 0; i < generator.single(0, 3); i++) {
//                            foods.emplace_back(std::move(generate_food()))->show();
//                        }
//                    }
//                }
//                if (utils::checkInvalidPosition(snake->grow((*it)->weight(), (*it)->weight()).show(), snake->nodes.back()) ==
//                    INVALID) {
//                    return gameOver(*snake);
//                }
//                snake->score += (*it)->weight();
//                api::soundEvent(LR"(\Media\Windows Proximity Notification.wav)");
//                continue;
//            }
//            it++;
//        }
#pragma endregion
