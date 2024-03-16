module;
#include "../include/config.hpp"
export module ancillarycat.utils;
import ancillarycat.entities;
import std;
namespace utils
{
	
export int checkInvalidPosition(Entity&, Entity&);
export int checkSnakeFood(const Snake&, const Food&);
export int checkOutofBound(const Entity&);
export std::chrono::seconds timer(const std::chrono::milliseconds& duration = std::chrono::milliseconds(INTERVALS));
export std::chrono::milliseconds elapsed(0);
export template <class _MyBase, class _MyDerived>
	requires std::is_base_of_v<_MyBase, _MyDerived>
bool instanceof(_MyBase*);
}

namespace utils
{
	
int checkOutofBound(const Entity& entity) {
	// as for snake, I think only to check the head is enough
	if (entity.y < START_ROW ||
		entity.y > START_ROW + BOX_HEIGHT - 1 ||
		entity.x < START_COL + 1 ||
		entity.x > START_COL + BOX_WIDTH) {
		return INVALID;
	}
	return VALID;
}

int checkSnakeFood(const Snake& snake, const Food& food) {
	if (snake.y == food.y && snake.x == food.x) {
		// TODO: implement the snake body check
		return INVALID;
	}
	return VALID;
}

int checkInvalidPosition(Entity& entity1, Entity& entity2) {
	// we can use 'static_cast' when we have the guarantee that the cast is safe and valid,
	//			otherwise, it may lead to undefined behavior.
	// In that case, we should use 'dynamic_cast' instead.
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

std::chrono::seconds timer(const std::chrono::milliseconds& duration)
{
	std::this_thread::sleep_for(duration);
	elapsed += duration;
	// cast milli to sec
	return std::chrono::duration_cast<std::chrono::seconds>(elapsed);
}
}