module;
#include "config.hpp"
export module ancillarycat.entities:entity;
import ancillarycat.console;
import std;
export class Entity
{
#pragma region my best friends
	//friend int checkInvalidPosition(Entity&, Entity&);
	//friend int checkOutofBound(const Entity&);
#pragma endregion
#pragma region Constructor
public:
	explicit Entity();
	explicit Entity(const Entity& entity);
	//explicit Entity(Entity&& entity) noexcept;
	explicit Entity(const int& generateTag, const char& _c);
	explicit Entity(const short& _y, const short& _x);
	explicit Entity(const short& _y, const short& _x, const char& _c);
	explicit Entity(const short& _y, const short& _x, const char& _c, const direction& _d);
	virtual ~Entity() = default;
#pragma endregion
#pragma region 'get' Function
public:
	[[nodiscard]] virtual constexpr char getChar() const noexcept final {
		return c;
	}
	[[nodiscard]] virtual constexpr short getY() const noexcept final {
		return y;
	}
	[[nodiscard]] virtual constexpr short getX() const noexcept final {
		return x;
	}
	[[nodiscard]] virtual constexpr direction getDirection() const noexcept final {
		return nDirection;
	}
	[[nodiscard]] virtual constexpr double getSpeed() const noexcept final {
		return speed;
	}
#pragma endregion
#pragma region Function
public:
	virtual Entity& show() noexcept;
	virtual Entity& regenerate() noexcept;
	virtual Entity& disappear() noexcept;
	virtual Entity& move(const short& offset = 1,const char& delimiter = ' ') noexcept;
#pragma endregion
#pragma region impl Function
protected:
	Entity& to_impl(const short& _y,const short& _x) noexcept;
	Entity& erase(const char& delimiter = ' ') noexcept;
#pragma endregion
#pragma region Member Variables
public:
	// I think it's ok to make the member variable public
	short y;
	short x;
	char c;
	direction nDirection;
	double speed;
	static unsigned short count;
#pragma endregion
};
export unsigned short Entity::count = 0;
#pragma region Utils
/*
* @brief check whether the base pointer is an instance of the derived class
* @param base the base pointer
* @return true if the base pointer is an instance of the derived class, otherwise false
* @note this can be viewed as the keyword `instanceof` in Java
*/
export template <class _MyBase, class _MyDerived>
	requires std::is_base_of_v<_MyBase, _MyDerived>
bool instanceof(_MyBase* base) {
	return dynamic_cast<_MyDerived*>(base) != nullptr;
}
#pragma endregion