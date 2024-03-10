module;

#include "config.hpp"
#include <Windows.h>
#include <mmsystem.h>

export module ancillarycat.snake;

import ancillarycat.print;
import ancillarycat.console;
import std;

export enum class direction;
export class Generator;
export class Food;
export class Snake;
export class Node;

enum class direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Generator {
public:
	static const inline direction direct() {
		std::mt19937 gen(device());
		std::uniform_int_distribution<int> distribution(0, 3);
		return static_cast<direction>(distribution(gen));
	}
	static const inline std::pair<short, short> coordinate(const short& y_lower_bound, const short& y_upper_bound, const short& x_lower_bound, const short& x_upper_bound) {
		std::mt19937 gen(device());
		std::uniform_int_distribution<short> y_distribution(y_lower_bound, y_upper_bound);
		std::uniform_int_distribution<short> x_distribution(x_lower_bound, x_upper_bound);
		return std::make_pair(y_distribution(gen), x_distribution(gen));
	}
	static const inline short single(const short& lower_bound, const short& upper_bound) {
		std::mt19937 gen(device());
		std::uniform_int_distribution<short> distribution(lower_bound, upper_bound);
		return distribution(gen);
	}
private:
	static std::random_device device;
}generator;

NO_EXPORT std::random_device Generator::device;

class Food {
public:
	Food() :
		y(generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 2)),
		x(generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2)),
		c('$') {}
public:
	// cannot add modifier `const`.
	inline Food& show() noexcept {
		consolePrint
			.setCursor(this->y, this->x)
			.print(this->c);
		return *this;
	}
	inline Food& regenrate() noexcept {
		y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 2);
		x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
		return *this;
	}
public:
	short y;
	short x;
	char c;
};

class Node {
public:
	Node() = default;
	explicit Node(short _y, short _x, direction _d) : y(_y), x(_x), nDirection(_d), c('O') {}
	explicit Node(short _y, short _x, direction _d, char _c) : y(_y), x(_x), nDirection(_d), c(_c) {}
public:
	// the coordinate of the node
	short y;
	short x;
	// the direction directs where the previous node is
	direction nDirection;
	// the character of the node
	char c;
};

class Snake {
public:
	Snake() = delete;
	explicit Snake(
		short y_lower_bound,
		short y_upper_bound,
		short x_lower_bound,
		short x_upper_bound) :
		hDirection(generator.direct()),
		y(generator.single(y_lower_bound, y_upper_bound)),
		x(generator.single(x_lower_bound, x_upper_bound)),
		c('@'),
		nodes({}),
		length(generator.single(2, 4)) {
		this->print();
	}
	explicit Snake(short _y, short _x) :
		hDirection(generator.direct()),
		y(_y),
		x(_x),
		c('@'),
		nodes({}),
		length(generator.single(2, 4)) {
		this->print();
	}
public:
	// snake head direction
	direction hDirection;
	// the coordinate of the snake head
	short y;
	short x;
	// the character of the snake head
	char c;
	// the node of the snake
	std::vector<Node> nodes;
	short length;
	short score;
public:
	void print() const noexcept {
		consolePrint
			.setCursor(this->y, this->x)
			.print(this->c);
	}
};

NO_EXPORT inline int checkInvalidPosition(const Snake& snake, const Food& food) {
	if (snake.y == food.y && snake.x == food.x)
		return INVALID;
	for (const Node& node : snake.nodes) {
		if (node.y == food.y && node.x == food.x) {
			return INVALID;
		}
	}
	return VALID;
}

namespace game {
export void snakeGame() {
	system("CLS");
	console.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH);
	Snake snake(START_ROW + 5, START_ROW + BOX_HEIGHT - 5, START_COL + 5, START_COL + BOX_WIDTH - 5);
	Food food;
	while (checkInvalidPosition(snake, food) == INVALID) {
		food.regenrate();
	}
	food.show();
}
}