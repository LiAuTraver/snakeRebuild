module;
#include "../include/config.hpp"
#include <conio.h>
#include  <Windows.h>
export module ancillarycat.entities:snake;
import :entity;
import ancillarycat.console;
import ancillarycat.api;
import std;
export class Snake;
export class Node;

class Node final : public Entity {
	friend class Snake;
public:
	explicit Node(const short& _y, const short& _x, const direction& _d) : Entity(_y, _x, 'o', _d)
	{
		speed = 1;
	}
	virtual ~Node() noexcept override {}
public:
};
class Snake final : public Entity {
public:
	explicit Snake() : Entity(), length(1), score(0) {
		speed = 1;
	}
	explicit Snake(const char& _c) :
		Entity(GENERATE_SNAKE, _c),
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
		for (auto& node : nodes) {
			node.show();
		}
		Entity::show();
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
	Snake& grow(const short& offset = 1, const short& add_score = -1) noexcept {
		// increase length and score
		// occurs when snake eat food
		if (add_score != -1) score += add_score;
		length += offset;
		for (short i = 0; i < offset; i++) {
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
                std::unreachable();
			}
		}
		return *this;
	}
	virtual Snake& move(const short& offset, const char& delimiter) noexcept override {
		const auto prevY = y;
		const auto prevX = x;
		Entity::move(offset, delimiter);
		nodes.insert(nodes.begin(), Node(prevY, prevX, nDirection));
		nodes.back().erase();
		nodes.pop_back();
		return *this;
	}
};