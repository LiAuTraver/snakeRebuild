module;

#include "../include/config.hpp"
#include <Windows.h>

export module ancillarycat.blocks:obstacle;

import ancillarycat.utils;
import ancillarycat.api;
import ancillarycat.console;
import std;
//
//export class Obstacle {
//public:
//    Obstacle(const char &_c) : c(_c) {
//        auto [y, x] = ancillarycat.api::Generator::coordinate(1, 23, 1, 79);
//        this->y = y;
//        this->x = x;
//    }
//
//    virtual ~Obstacle() noexcept {}
//
//    void show() const {
//        std::cout << "\033[" << y << ";" << x << "H" << c;
//    }
//
//    void hide() const {
//        std::cout << "\033[" << y << ";" << x << "H ";
//    }
//#pragma region useless Function
//public:
//    NODISCARD virtual constexpr inline SHORT getY() const noexcept final {
//        return y;
//    }
//    NODISCARD virtual constexpr inline SHORT getX() const noexcept final {
//        return x;
//    }
//    NODISCARD virtual constexpr inline char getChar() const noexcept final {
//        return c;
//    }
//
//#pragma endregion
//
//public:
//    short y;
//    short x;
//    char c;
//};