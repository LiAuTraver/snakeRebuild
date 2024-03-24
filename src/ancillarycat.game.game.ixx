module;
#pragma warning(disable : 4244)
#include "../include/config.hpp"

export module ancillarycat.game:game;

import :leaderboard;
import ancillarycat.utils;
import ancillarycat.console;
import ancillarycat.api;
import ancillarycat.entities;
import ancillarycat.blocks;
import std;

namespace game {
constinit std::unique_ptr <Snake> snake = nullptr;
std::vector <std::unique_ptr<Food>> foods{};
std::vector <std::unique_ptr<Block>> blocks{};
constinit bool isIgnore = false;
constinit int prevKey = -2;
constinit long long START_INTERVAL = 250; // Start long longerval in milliseconds
constinit long long MIN_INTERVAL = 100;   // Minimum long long in milliseconds

inline void score_logger(const short& score) {
	const std::string current_score = "Score: " + std::to_string(score);
	const auto len = static_cast<short>((INFO_WIDTH - current_score.length()) / 2 + INFO_COL);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(INFO_ROW + 1, len, current_score, ansiColor::redIntense, ansiBackground::black);
}

inline void time_logger(const std::chrono::seconds& cur, const short& row, const short& col) {
	const std::string elapsed = "Time: " + std::to_string(cur.count()) + "s";
	const auto len = static_cast<short>((INFO_WIDTH - elapsed.length()) / 2 + col);
	console
		.setStyle(ansiStyle::blink)
		.shuttle(row, len, elapsed, ansiColor::white, ansiBackground::black);
}

long long intervalChange() {
	const short startLength = 10;    // Length at which to start decreasing the shorterval
	const short targetLength = 30;   // Length at which the interval approaches MIN_INTERVAL but doesn't reach it

	if (snake->length <= startLength) {
		return START_INTERVAL;
	}

	// Calculate the frame interval based on snake's length
	// The formula uses linear interpolation between START_INTERVAL and MIN_INTERVAL
	// and adjusts it to ensure it never reaches MIN_INTERVAL exactly
	INTERVAL = START_INTERVAL - static_cast<int>((static_cast<double>(snake->length - startLength) / (targetLength - startLength)) * (START_INTERVAL - MIN_INTERVAL));

	// Ensure the interval never goes below MIN_INTERVAL
	INTERVAL = std::max(INTERVAL, MIN_INTERVAL + 1);
	return INTERVAL;
}
NO_EXPORT int gameOver() {
	api::soundEvent(LR"(\Media\Windows Critical Stop.wav)", soundFlag::async, soundFlag::async);
	console
		.centeredShuttle(console.height - 1, "Game Over!", ansiColor::red, ansiBackground::black, 30000)
		.bot("Press 'q' to return to menu", ansiColor::white, ansiBackground::black, 30000)
		.setCursorCoordinate(0, 0);
	if (rankBuffer.size() < 10 || snake->score > std::get<1>(*rankBuffer.rbegin()))
		return game::writeBuffer(snake->score, utils::elapsed);
	while(console.getch() != 'q');
	blocks.clear();
	foods.clear();
	snake = nullptr;
	return GAMEOVER;
}

NO_EXPORT int gameWinning() {
	api::soundEvent(LR"(\Media\Windows Notify System Generic.wav)", soundFlag::async, soundFlag::async);
	game::time_logger(utils::timer(std::chrono::milliseconds(game::intervalChange())), INFO_ROW, INFO_COL);
	game::score_logger(WIN_SCORE);
	console
		.centeredShuttle(console.height - 1, "You Win!", ansiColor::green, ansiBackground::black, 30000)
		.bot("Press 'q' to return to menu", ansiColor::white, ansiBackground::black, 30000)
		.setCursorCoordinate(0, 0);
	if (rankBuffer.size() < 10 || WIN_SCORE > std::get<1>(*rankBuffer.rbegin()))
		return game::writeBuffer(WIN_SCORE, utils::elapsed);
	while(console.getch() != 'q');
	blocks.clear();
	foods.clear();
	snake = nullptr;
	return GAMEWIN;
}

NODISCARD NO_EXPORT inline int input_key() {
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

// (boost library)std::bitset need to know the size at compile time, so might not be a good choice here.
NODISCARD inline NO_EXPORT std::vector<std::vector<bool>> flip_flap_map(const std::vector<std::vector<double>>& map, const double& threshold) {
	std::vector<std::vector<bool>> flip_flap(map.size(), std::vector<bool>(map.at(0).size(), false));
	for (size_t r = 0; r < map.size(); r++)
		for (size_t c = 0; c < map.at(0).size(); c++)
			if (map.at(r).at(c) < threshold)
				flip_flap.at(r).at(c) = true;
	return flip_flap;
}

NO_EXPORT inline std::unique_ptr <Food> generate_valid_food() {
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
		if (flag) {
			flag = false;
			continue;
		}
		// TODO: obstacle
		for (size_t k = 0; k < blocks.size(); k++) {
			if (utils::checkInvalidPosition(*p, *blocks.at(k)) == INVALID) {
				p = generate_food();
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			continue;
		}
		// if all the conditions are met, break the loop
		break;
	}
	return std::move(p);
}
void gameInit() {
	console.clear();
	snake = std::make_unique<Snake>(Snake('@'));
	foods.emplace_back(generate_valid_food())->show();
	std::ranges::for_each(foods, [](const auto& food) { food->show(); });
	isIgnore = false;
	prevKey = -2;
	INTERVAL = START_INTERVAL;
	MIN_INTERVAL = static_cast<long long>(INTERVAL / 2.5);
	api::soundEvent(LR"(\Media\Ring01.wav)");
	console
		.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH)
		.box(INFO_ROW, INFO_COL, INFO_HEIGHT, INFO_WIDTH);
	if (ENABLE_OBSTACLE) {
		// generate random obstacles
		auto flip_flap = flip_flap_map(generator.heightMap(BOX_HEIGHT, BOX_WIDTH), OBSTACLE_THRESHOLD);
		for (size_t r = 0; r < flip_flap.size(); r++)
			for (size_t c = 0; c < flip_flap.at(0).size(); c++)
				if (flip_flap.at(r).at(c)) {
					blocks.emplace_back(std::make_unique<Obstacle>(Obstacle{ static_cast<short>(r + START_ROW), static_cast<short>(c + START_COL + 1), '#' }));
					if (utils::checkInvalidPosition(*snake, *blocks.back()) == INVALID)
						blocks.pop_back();
					for (size_t k = 0; k < foods.size(); k++)
						if (utils::checkInvalidPosition(*foods.at(k), *blocks.back()) == INVALID)
							blocks.pop_back();
					for (const auto& node : snake->nodes)
						if (utils::checkInvalidPosition(node, *blocks.back()) == INVALID)
							blocks.pop_back();
				}
		std::ranges::for_each(blocks, [](const auto& block) { block->show(); });
	}
	for (int i = 0; i < 3; i++) {
		console.bot("Game will start in " + std::to_string(3 - i) + " seconds", ansiColor::green,
			ansiBackground::black);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	console
		.fillLine(' ', 1, true)
		.setStyle(ansiStyle::blink)
		.centeredShuttle(console.height - 1, "Game Start!", ansiColor::green, ansiBackground::black, 3000);
}

int snakeGame() {
	game::gameInit();
	while (true) {
		if (console.getch(true) && input_key()) continue;
		if (snake->move(1, 'o').check() == INVALID || utils::checkOutofBound(*snake) == INVALID)return gameOver();
		for (size_t i = 0; i < foods.size(); i++)
			if (utils::checkInvalidPosition(*snake, *foods.at(i)) == INVALID) {
				if (snake->grow(foods.at(i)->weight(), foods.at(i)->weight()).show().check() == INVALID)
					return gameOver();
				if (WIN_SCORE > 0 && snake->score >= WIN_SCORE)return gameWinning();
				foods.erase(foods.begin() + i);
				api::soundEvent(LR"(\Media\Windows Proximity Notification.wav)");
				if (foods.size() <= FOOD_MAX_COUNT) {
					const int genNum = generator.single(0, 1) || foods.empty() ? generator.single(0, 3) : 0;
					for (size_t j = 0; j < genNum; j++) {
						foods.emplace_back(generate_valid_food())->show();
					}
				}
			}
		for (size_t i = 0; i < blocks.size(); i++)
			if (utils::checkInvalidPosition(*snake, *blocks.at(i)) == INVALID)return gameOver();
		if (foods.empty()) foods.emplace_back(generate_valid_food())->show();
		if (isIgnore) isIgnore = false;
		game::time_logger(utils::timer(std::chrono::milliseconds(game::intervalChange())), INFO_ROW, INFO_COL);

		game::score_logger(snake->score);
	}
	std::unreachable();
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
