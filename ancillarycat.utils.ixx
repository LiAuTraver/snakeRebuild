module;
#include "config.hpp"
export module ancillarycat.utils;
import ancillarycat.entities;
export int checkInvalidPosition(Entity&, Entity&);
export int checkSnakeFood(const Snake&, const Food&);
export int checkOutofBound(const Entity&);

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