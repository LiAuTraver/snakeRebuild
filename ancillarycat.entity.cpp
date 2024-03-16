module;
#include "config.hpp"
module ancillarycat.entities:entity;
import ancillarycat.console;
import ancillarycat.api;
import std;

//  due to the intellisense bug, the class definition below could not be recognized, so I temporarily use preprocessor directives.
#ifndef module_bug
Entity::Entity() : y(0), x(0), c(' '), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
Entity::Entity(const Entity& entity) : y(entity.y), x(entity.x), c(entity.c), nDirection(entity.nDirection), speed(entity.speed) { count++; }
//Entity::Entity(Entity&& entity) noexcept : y(entity.y), x(entity.x), c(entity.c), nDirection(entity.nDirection), speed(entity.speed) {
//	entity.y = 0;
//	entity.x = 0;
//	entity.c = ' ';
//	entity.nDirection = direction::NO_DIRECTION;
//	entity.speed = 0;
//	count++;
//}
Entity::Entity(const int& generateTag, const char& _c) : c(_c), speed(0) {
	switch (generateTag) {
	case GENERATE_SNAKE:
		y = generator.single(START_ROW + 5, START_ROW + BOX_HEIGHT - 5);
		x = generator.single(START_COL + 5, START_COL + BOX_WIDTH - 5);
		nDirection = generator.direct();
		break;
	case GENERATE:
		y = generator.single(START_ROW, START_ROW + BOX_HEIGHT);
		x = generator.single(START_COL, START_COL + BOX_WIDTH);
		nDirection = generator.direct();
		break;
	case GENERATE_COOR:
		y = generator.single(START_ROW, START_ROW + BOX_HEIGHT);
		x = generator.single(START_COL, START_COL + BOX_WIDTH);
		nDirection = direction::NO_DIRECTION;
	case GENERATE_DIR:
		y = 0;
		x = 0;
		nDirection = generator.direct();
	default:
		std::unreachable();
	}
	count++;
}
Entity::Entity(const short& _y, const short& _x) : y(_y), x(_x), c(' '), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
Entity::Entity(const short& _y, const short& _x, const char& _c) : y(_y), x(_x), c(_c), nDirection(direction::NO_DIRECTION), speed(0) { count++; }
Entity::Entity(const short& _y, const short& _x, const char& _c, const direction& _d) : y(_y), x(_x), c(_c), nDirection(_d), speed(0) { count++; }

Entity& Entity::show() noexcept {
	console
		.setCursorCoordinate(this->y, this->x)
		.print(std::string{ this->c });
	return *this;
}
Entity& Entity::regenerate() noexcept {
	y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 1);
	x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
	return *this;
}
// make the char ' ' and erase previous char
Entity& Entity::disappear() noexcept {
	this->c = ' ';
	return this->erase();
}
// move and clear and show
Entity& Entity::move(const short& offset, const char& delimiter) noexcept
{
	this->erase(delimiter);
	switch (nDirection)
	{
	case direction::UP:
		y -= static_cast<short>(offset * speed);
		break;
	case direction::DOWN:
		y += static_cast<short>(offset * speed);
		break;
	case direction::LEFT:
		x -= static_cast<short>(offset * speed);
		break;
	case direction::RIGHT:
		x += static_cast<short>(offset * speed);
		break;
	default:
		[[unlikely]];
		//exit(INVALID_DIRECTION_INPUT); // sth wrong here
	}
	return this->to_impl(y, x);
}
Entity& Entity::to_impl(const short& _y, const short& _x) noexcept
{
	console
		.set_cur_coor_impl(_y, _x);
	std::print("{}", c);
	return *this;
}
Entity& Entity::erase(const char& delimiter) noexcept
{
	console
		.set_cur_coor_impl(y, x);
	std::print("{}",delimiter);
	return *this;
}
#endif


