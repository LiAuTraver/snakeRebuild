module;

#include "config.hpp"
#include <Windows.h>
#include <mmsystem.h>

export module ancillarycat.snake;

import ancillarycat.print;
import ancillarycat.console;
import std;

export enum class direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

NO_EXPORT class Generator {
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
	static const inline short axis(const short& lower_bound, const short& upper_bound) {
		std::mt19937 gen(device());
		std::uniform_int_distribution<short> distribution(lower_bound, upper_bound);
		return distribution(gen);
	}
private:
	static std::random_device device;
}generator;

NO_EXPORT class Food {
public:
	short y;
	short x;
};

NO_EXPORT class Node {
public:
	Node() = delete;
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

export class Snake {
public:
	Snake() = delete;
	explicit Snake(
		short y_lower_bound,
		short y_upper_bound,
		short x_lower_bound,
		short x_upper_bound) :
		hDirection(generator.direct()),
		y(generator.axis(y_lower_bound, y_upper_bound)),
		x(generator.axis(x_lower_bound, x_upper_bound)),
		c('@'),
		nodes({})
	{}
	explicit Snake(short _y, short _x) :
		hDirection(generator.direct()),
		y(_y),
		x(_x),
		c('@'),
		nodes({})
	{}
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
};

namespace game
{
export void snakeGame() {
	console
		.setCursorCoordinate(0, 0);
	system("CLS");
	SHORT& width = console.width;
	SHORT& height = console.height;
	console.box(5, 5, 10, 10);
	//Snake snake;
}
}