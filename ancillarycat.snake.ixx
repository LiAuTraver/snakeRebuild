module;

#include "config.hpp"
#include <Windows.h>

export module ancillarycat.snake;

import ancillarycat.print;
import ancillarycat.console;
import std;

export enum class direction;
export class Generator;
export class Entity;
export class Food;
export class Snake;
export class Node;
export template <class _MyBase, class _MyDerived> inline bool instanceof(const _MyBase*);

NO_EXPORT int checkInvalidPosition(const Entity&, const Entity&);
NO_EXPORT int checkOutofBound(const Entity&);
NO_EXPORT inline int checkSnakeFoodInvalidPosition(Snake&, Food&);

// definition goes below.

template <class _MyBase, class _MyDerived>
inline bool instanceof(const _MyBase* base) {
	return dynamic_cast<const _MyDerived*>(base) != nullptr;
}

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

class Entity {
	friend int checkInvalidPosition(Entity&, Entity&);
	friend int checkOutofBound(const Entity&);
	friend int checkSnakeFoodInvalidPosition(Snake&, Food&);
public:
	Entity() = default;
	explicit Entity(const int& isGenerate, const char& _c) : c(_c) {
		if (isGenerate == GENERATE) {
			y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 2);
			x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
		}
	}
	explicit Entity(short _y, short _x) : y(_y), x(_x) {}
	explicit Entity(short _y, short _x, char _c) : y(_y), x(_x), c(_c) {}
	virtual ~Entity() = default;
public:
	[[nodiscard]] virtual constexpr inline const char getChar() const noexcept {
		return c;
	}
	[[nodiscard]] virtual constexpr inline const short getY() const noexcept {
		return y;
	}
	[[nodiscard]] virtual constexpr inline const short getX() const noexcept {
		return x;
	}
	virtual inline Entity& show() noexcept {
		consolePrint
			.setCursor(this->y, this->x)
			.print(this->c);
		return *this;
	}
	virtual inline Entity& regenrate() noexcept {
		y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 2);
		x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
		return *this;
	}
public:
	short y;
	short x;
	char c;
};

class Food : public Entity {
public:
	Food() : Entity(GENERATE, '$') {}
};

class Node : public Entity {
public:
	Node() = default;
	explicit Node(short _y, short _x, direction _d) : Entity(_y, _x, 'o'), nDirection(_d) {}
	explicit Node(short _y, short _x) : Entity(_y, _x, 'o') {}
	explicit Node(short _y, short _x, char _c, direction _d) : Entity(_y, _x, _c), nDirection(_d) {}
private:
	// the direction directs where the previous node is
	direction nDirection;
};

class Snake : public Entity {
public:
	Snake() = delete;
	explicit Snake(
		const short& y_lower_bound, const short& y_upper_bound,
		const short& x_lower_bound, const short& x_upper_bound,
		const char& _c) :
		Entity(GENERATE, _c),
		hDirection(generator.direct()),
		length(1),
		score(0) {
		// check snake direction and put the following node
		switch (hDirection) {
		case direction::UP:
			for (short i = 1; i < generator.single(3, 6); i++)
				nodes.emplace_back(Node(y + i, x, direction::UP));
			break;
		case direction::DOWN:
			for (short i = 1; i < generator.single(3, 6); i++)
				nodes.emplace_back(Node(y - i, x, direction::DOWN));
			break;
		case direction::LEFT:
			for (short i = 1; i < generator.single(3, 6); i++)
				nodes.emplace_back(Node(y, x + i, direction::LEFT));
			break;
		case direction::RIGHT:
			for (short i = 1; i < generator.single(3, 6); i++)
				nodes.emplace_back(Node(y, x - i, direction::RIGHT));
			break;
		default:
			[[unreachable]] break;
		}
		this->show();
	}
public:
	// snake head direction
	direction hDirection;
	// the node of the snake
	std::vector<Node> nodes;
	short length;
	short score;
public:
	virtual inline Snake& show() noexcept override {
		Entity::show();
		// TODO: show the snake body
		for (auto& node : nodes) {
			node.show();
		}
		return *this;
	};
public:
	inline Snake& grow(const short& offset = 1) noexcept {
		// todo: add a node, and set the direction of the node
		// increase length and score
		// occurs when snake eat food		
		return *this;
	}
	// TODO: implement the move and changeDirection
	//inline Snake& move(const short& offset = 1) noexcept;
	//inline Snake& changeDirection(const direction& _d) noexcept;
};

NO_EXPORT inline int checkOutofBound(const Entity& entity) {

}

NO_EXPORT inline int checkSnakeFoodInvalidPosition(Snake& snake, Food& food) {
	if (snake.y == food.y && snake.x == food.x) {
	return INVALID;
	}
	else
	{
		return VALID;
	}
}
NO_EXPORT inline int checkInvalidPosition(Entity& entity1, Entity& entity2) {
	if (instanceof<Entity, Snake>(&entity1) && instanceof<Entity, Food>(&entity2)) {
		if (entity1.y == entity2.y && entity1.x == entity2.x) {
			return INVALID;
		}
		// TODO: implement the snake body
	}
	else if (entity1.y == entity2.y && entity1.x == entity2.x) {
		return INVALID;
	}
	else return VALID;
}

namespace game {
export void snakeGame() {
	system("CLS");
	console.box(START_ROW, START_COL, BOX_HEIGHT, BOX_WIDTH);
	Snake snake(START_ROW + 5, START_ROW + BOX_HEIGHT - 5, START_COL + 5, START_COL + BOX_WIDTH - 5, '@');
	Food food;
	Node node(20, 20);
	while (checkInvalidPosition(snake, food) == INVALID) {
		food.regenrate();
	}
	food.show();
}
}