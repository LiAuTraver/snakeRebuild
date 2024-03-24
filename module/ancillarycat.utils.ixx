module;
#include "../include/config.hpp"
export module ancillarycat.utils;
import ancillarycat.entities;
import ancillarycat.console;
import std;
namespace utils
{
export int checkInvalidPosition(const Point&, const Point&);
export int checkOutofBound(const Point&);
export std::chrono::seconds timer(const std::chrono::milliseconds& duration = std::chrono::milliseconds(INTERVAL));
export std::chrono::milliseconds elapsed(0);
export template <class _MyBase, class _MyDerived>
	requires std::is_base_of_v<_MyBase, _MyDerived>
bool instanceof(_MyBase*);

}