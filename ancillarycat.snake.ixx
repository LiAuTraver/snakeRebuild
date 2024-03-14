module;
#include "config.hpp";
export module ancillarycat.snake;
import <conio.h>;
import <Windows.h>;
import ancillarycat.console;
import ancillarycat.windows.api;
import ancillarycat.leaderboard;
import ancillarycat.generator;
import ancillarycat.entity;
import std;
#pragma region export
enum class direction;
export class Generator;
export class Entity;
export class Food;
export class Snake;
export class Node;
export template <class _MyBase, class _MyDerived> bool instanceof(_MyBase*);
NO_EXPORT int checkInvalidPosition(Entity&, Entity&);
NO_EXPORT int checkSnakeFood(const Snake&, const Food&);
NO_EXPORT int checkOutofBound(const Entity&);
constinit int time = 0; // for debugging
namespace game
{
NO_EXPORT void clock(const std::chrono::seconds&, const int&, const int&);
NO_EXPORT int gameOver();
NO_EXPORT void gameInit();
export int snakeGame();
}
#pragma endregion

// definition goes below.
/*
* @brief check whether the base pointer is an instance of the derived class
* @param base the base pointer
* @return true if the base pointer is an instance of the derived class, otherwise false
* @note this can be viewed as the keyword `instanceof` in Java
*/
template <class _MyBase, class _MyDerived>
	requires std::is_base_of_v<_MyBase, _MyDerived>
bool instanceof(_MyBase* base) {
	return dynamic_cast<_MyDerived*>(base) != nullptr;
}

class Food final : public Entity {
public:
	// as now food is simple, I don't think it's necessary to have lots of ctor
	explicit Food(const char& _c) : Entity(GENERATE, _c) {
		Entity::show();
	}
	// (temporary) disable the move function for food
	Food& move([[maybe_unused]] const short& offset = 1) noexcept override {
		return *this;
	}
	virtual ~Food() noexcept override {}
};

class Node final : public Entity {
	friend class Snake;
public:
	explicit Node(const short& _y, const short& _x, const direction& _d) : Entity(_y, _x, 'o', _d) {}
	virtual ~Node() noexcept override {}
public:
};

class Snake final : public Entity {
public:
	explicit Snake() : Entity(), length(1), score(0) {
		speed = 1;
	}
	explicit Snake(const char& _c) :
		Entity(GENERATE, _c),
		length(1),
		score(0) {
		speed = 1;
		// check snake direction and put the following node
		for (short i = 1; i < generator.single(3, 5); i++)
			nodes.emplace_back(Node(y + i, x, nDirection));
		this->Snake::show();
	}
	virtual ~Snake() noexcept override {}
public:
	// the node of the snake
	std::vector<Node> nodes;
	short length;
	short score;
public:
	virtual Snake& show() noexcept override {
		Entity::show();
		for (auto& node : nodes) {
			node.show();
		}
		return *this;
	};
	int check() const  noexcept
	{
		for (const auto& node : this->nodes) {
			if (node.y == this->y && node.x == this->x) {
				return INVALID;
			}
		}
		return VALID;
	}
	// NOTE: the return type is std::expected
	int grow(const short& offset = 1, const short& add_score = -1) noexcept {
		// TODO: add a node, and set the direction of the node
		// increase length and score
		// occurs when snake eat food
		if (add_score != -1) score += add_score;
		length += offset;
		for (short i = 0; i < offset; i++) {
			// TODO: implement the growth function
			switch (nodes.back().nDirection)
			{
			case direction::UP:
				nodes.emplace_back(Node(nodes.back().y + 1, nodes.back().x, nodes.back().nDirection));
				break;
			case direction::DOWN:
				nodes.emplace_back(Node(nodes.back().y - 1, nodes.back().x, nodes.back().nDirection));
				break;
			case direction::LEFT:
				nodes.emplace_back(Node(nodes.back().y, nodes.back().x + 1, nodes.back().nDirection));
				break;
			case direction::RIGHT:
				nodes.emplace_back(Node(nodes.back().y, nodes.back().x - 1, nodes.back().nDirection));
				break;
			default:
				[[unlikely]];
				//exit(INVALID_DIRECTION_INPUT);
			}
			if (checkInvalidPosition(*this, nodes.back()) == INVALID)
			{
				return INVALID;
			}
		}
		return VALID;
	}
	Snake& moveNodeSeq(const short& offset = 1)
	{
		// for the first node, first check the direction of the snake,
		// if the direction is not the same as the snake, then change the direction of the node
		// and move the node forward
		if (nodes.empty())return *this;
		auto it = nodes.begin();
		it->move(offset);
		if (it->nDirection != this->nDirection) {
			it->nDirection = this->nDirection;
		}
		if (it >= nodes.end() - 1) {
			return *this;
		}
		for (it = nodes.begin() + 1; it != nodes.end(); ++it) {
			// for the rest of the nodes, check the previous node's direction,
			// if the direction is not the same as the previous node, then change the direction of the node
			// and move the node forward
			it->move(offset);
			if (it->nDirection != (it - 1)->nDirection) {
				it->nDirection = (it - 1)->nDirection;
			}
		}
		// for the last `offset` nodes, simply discard them
		// currently only implement the offset = 1, so here might be blank.
		return *this;
	}
	virtual Snake& move(const short& offset = 1) noexcept override {
		Entity::move(offset);
		// TODO: implement body movement
		// move the body of the snake
		moveNodeSeq(offset);
		// debug
		console.setCursorCoordinate(0, 0);
		console.top("You pressed a key.")
			.setCursorCoordinate(1, console.width / 3);
		std::print("Now you've pressed {} keys in total.", time++);
		console.setCursorCoordinate(y, x);
		return *this;
	}
	//inline Snake& changeDirection(const direction& _d) noexcept;
};


//NO_EXPORT DEFINITION template <typename _It>
//	requires std::bidirectional_iterator<_It>
//Node& prevNode(_It it) {
//	return *std::prev(it);
//}
NO_EXPORT DEFINITION inline int checkOutofBound(const Entity& entity) {
	// as for snake, I think only to check the head is enough
	if (entity.y < START_ROW + 1 ||
		entity.y > START_ROW + BOX_HEIGHT - 1 ||
		entity.x < START_COL + 1 ||
		entity.x > START_COL + BOX_WIDTH - 2) {
		return INVALID;
	}
	return VALID;
}
NO_EXPORT DEFINITION int checkSnakeFood(const Snake& snake, const Food& food) {
	if (snake.y == food.y && snake.x == food.x) {
		// TODO: implement the snake body check
		return INVALID;
	}
	return VALID;
}

// we can use 'static_cast' when we have the guarantee that the cast is safe and valid,
//			otherwise, it may lead to undefined behavior.
// In that case, we should use 'dynamic_cast' instead.
NO_EXPORT DEFINITION int checkInvalidPosition(Entity& entity1, Entity& entity2) {
	if (entity1.y == entity2.y && entity1.x == entity2.x) {
		return INVALID;
	}
	if (instanceof<Entity, Snake>(&entity1) && instanceof<Entity, Food>(&entity2)) {
		return checkSnakeFood(static_cast<Snake&>(entity1), static_cast<Food&>(entity2));
	}
	if (instanceof<Entity, Food>(&entity1) && instanceof<Entity, Snake>(&entity2)) {
		return checkSnakeFood(static_cast<Snake&>(entity2), static_cast<Food&>(entity1));
	}
	return VALID;
}

namespace game
{
NO_EXPORT DEFINITION void clock(const std::chrono::seconds& cur, const int& row = TIMER_ROW, const int& col = TIMER_COL) {
	console
		.printAndReturn(TIMER_ROW, TIMER_COL, std::to_string(cur.count()));
}
NO_EXPORT DEFINITION int gameOver() {
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
NO_EXPORT DEFINITION void gameInit() {
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
			if(snake.grow() == INVALID)
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
	std::unreachable();
}
}