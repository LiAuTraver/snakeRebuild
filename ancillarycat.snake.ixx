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
import ancillarycat.ansi;
import std;
#pragma region export
export class Food;
export class Snake;
export class Node;
constinit int time = 0; // for debugging
export int checkInvalidPosition(Entity&, Entity&);
export int checkSnakeFood(const Snake&, const Food&);
export int checkOutofBound(const Entity&);
#pragma endregion

// definition goes below.

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

DEFINITION int checkOutofBound(const Entity& entity) {
	// as for snake, I think only to check the head is enough
	if (entity.y < START_ROW + 1 ||
		entity.y > START_ROW + BOX_HEIGHT - 1 ||
		entity.x < START_COL + 1 ||
		entity.x > START_COL + BOX_WIDTH - 2) {
		return INVALID;
	}
	return VALID;
}
DEFINITION int checkSnakeFood(const Snake& snake, const Food& food) {
	if (snake.y == food.y && snake.x == food.x) {
		// TODO: implement the snake body check
		return INVALID;
	}
	return VALID;
}
// we can use 'static_cast' when we have the guarantee that the cast is safe and valid,
//			otherwise, it may lead to undefined behavior.
// In that case, we should use 'dynamic_cast' instead.
DEFINITION int checkInvalidPosition(Entity& entity1, Entity& entity2) {
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