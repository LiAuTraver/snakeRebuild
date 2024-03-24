module;
#include "../include/config.hpp"
module ancillarycat.utils;
import ancillarycat.entities;
import ancillarycat.console;
import ancillarycat.blocks;
namespace utils
{

int checkOutofBound(const Point& point) {
	// as for snake, I think only to check the head is enough
	if (point.y < START_ROW ||
		point.y > START_ROW + BOX_HEIGHT - 1 ||
		point.x < START_COL + 1 ||
		point.x > START_COL + BOX_WIDTH) {
		return INVALID;
	}
	return VALID;
}


int checkInvalidPosition(const Point& point1, const Point& point2) {
	if (point1.getY() == point2.getY() && point1.getX() == point2.getX()) {
		return INVALID;
	}
	// we can use 'static_cast' when we have the guarantee that the cast is safe and valid,
	//			otherwise, it may lead to undefined behavior.
	// In that case, we should use 'dynamic_cast' instead.
	// bug here, see the definition of `instanceof`
	//if (instanceof<Entity, Snake>(&point1) && instanceof<Entity, Food>(&point2)) {
	//	return checkSnakeFood(static_cast<Snake&>(point1), static_cast<Food&>(point2));
	//}
	//if (instanceof<Entity, Food>(&point1) && instanceof<Entity, Snake>(&point2)) {
	//	return checkSnakeFood(static_cast<Snake&>(point2), static_cast<Food&>(point1));
	//}
	return VALID;
}

/*
* @brief check whether the base pointer is an instance of the derived class
* @param base the base pointer
* @return true if the base pointer is an instance of the derived class, otherwise false
* @note this can be viewed as the keyword `instanceof` in Java
* @note there are some bug here, dynamic_cast resulting in reading access violation
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