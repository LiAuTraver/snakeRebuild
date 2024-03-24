module;
#include "../include/config.hpp"
module ancillarycat.entities:entity;
import ancillarycat.console;
import ancillarycat.api;
import std;

//  due to the intellisense bug, the class definition below could not be recognized, so I temporarily use preprocessor directives.

import ancillarycat.api;

Entity::Entity() : Point(), speed(0) { countEntity++; }

Entity::Entity(const Entity &entity) : Point(entity) {
    speed = entity.speed;
    countEntity++;
}

Entity::Entity(const int &generateTag, const char &_c) : Point(generateTag, _c) {
    countEntity++;
}

Entity::Entity(const short &_y, const short &_x) : Point(_y, _x, ' '), speed(0) {
    countEntity++;
}

Entity::Entity(const short &_y, const short &_x, const char &_c) : Point(_y, _x, _c), speed(0) {
    countEntity++;
}

Entity::Entity(const short &_y, const short &_x,
               const char &_c, const direction &_d)
        : Point(_y, _x, _c, _d), speed(0) { countEntity++; }

Entity &Entity::regenerate() noexcept {
    y = generator.single(START_ROW + 1, START_ROW + BOX_HEIGHT - 1);
    x = generator.single(START_COL + 1, START_COL + BOX_WIDTH - 2);
    return *this;
}

// make the char ' ' and erase previous char
Entity &Entity::disappear() noexcept {
    this->c = ' ';
    return this->erase();
}

// move and clear and show
Entity &Entity::move(const short &offset, const char &delimiter) noexcept {
    this->erase(delimiter);
    switch (nDirection) {
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

Entity &Entity::to_impl(const short &_y, const short &_x) noexcept {
    console
            .set_cur_coor_impl(_y, _x);
    std::print("{}", c);
    return *this;
}

Entity &Entity::erase(const char &delimiter) noexcept {
    console
            .set_cur_coor_impl(y, x);
    std::print("{}", delimiter);
    return *this;
}


