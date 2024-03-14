module;
#include "config.hpp"
export module ancillarycat.entity;
import ancillarycat.console;
import ancillarycat.ansi;
import std;
// ReSharper disable once CppClassCanBeFinal
export class Entity
{
#pragma region my best friends
	friend int checkInvalidPosition(Entity&, Entity&);
	friend int checkOutofBound(const Entity&);
#pragma endregion
#pragma region Constructor
public:
	explicit Entity();
	explicit Entity(const Entity& entity);
	explicit Entity(Entity&& entity) noexcept;
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
public:
	virtual Entity& show() noexcept;
	virtual Entity& regenerate() noexcept;
	virtual Entity& disappear() noexcept;
	virtual Entity& move(const short& offset = 1) noexcept;
#pragma region impl Function
private:
	Entity& move_impl(const short& offset = 1) noexcept;
	Entity& erase() noexcept;
#pragma endregion
public:
	// I think it's ok to make the member variable public
	short y;
	short x;
	char c;
	direction nDirection;
	double speed;
	static unsigned short count;
};
export unsigned short Entity::count = 0;