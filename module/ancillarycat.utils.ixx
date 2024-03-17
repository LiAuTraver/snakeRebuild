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