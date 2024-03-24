module;

#include "../include/config.hpp"

export module ancillarycat.entities:entity;

import ancillarycat.console;
import ancillarycat.api;
import std;

export class Entity : public Point {
#pragma region my best friends
    //friend int checkInvalidPosition(Entity&, Entity&);
    //friend int checkOutofBound(const Entity&);
#pragma endregion
#pragma region Constructor
public:
    explicit Entity();

    // copy constructor may not be explicit
    Entity(const Entity &entity);

    explicit Entity(const int &generateTag, const char &_c);

    explicit Entity(const short &_y, const short &_x);

    explicit Entity(const short &_y, const short &_x, const char &_c);

    explicit Entity(const short &_y, const short &_x, const char &_c, const direction &_d);

    virtual ~Entity() noexcept {
        countEntity--;
    }

#pragma endregion
#pragma region 'get' Function
public:

    [[nodiscard]] virtual constexpr double getSpeed() const noexcept final {
        return speed;
    }

    [[nodiscard]] virtual constexpr unsigned short count() const noexcept {
        return countEntity;
    }

#pragma endregion
#pragma region Function
public:

    virtual Entity &regenerate() noexcept;

    virtual Entity &disappear() noexcept;

    virtual Entity &move(const short &offset = 1, const char &delimiter = ' ') noexcept;

#pragma endregion
#pragma region impl Function
protected:
    Entity &to_impl(const short &_y, const short &_x) noexcept;

    Entity &erase(const char &delimiter = ' ') noexcept;

#pragma endregion
#pragma region Member Variables
public:
    // I think it's ok to make the member variable public
    double speed;
    static unsigned short countEntity;
#pragma endregion
};
unsigned short Entity::countEntity = 0;
