module;
#include "../include/config.hpp"
export module ancillarycat.console:point;
import :console;
import std;
import ancillarycat.api;

class Point{
public:
    explicit Point() : x(0), y(0), c(' '), nDirection(direction::NO_DIRECTION) {}
    Point(const Point &point) : y(point.y), x(point.x), c(point.c), nDirection(point.nDirection) {}
    explicit Point(const short &_y, const short &_x, const char& _c) : y(_y), x(_x), c(_c), nDirection(direction::NO_DIRECTION) {}
    explicit Point(const short &_y, const short &_x, const char& _c, const direction& _d) : y(_y), x(_x), c(_c), nDirection(_d) {}
    explicit Point(const int& generateTag, const char& _c): c(_c) {
        switch (generateTag) {
            case GENERATE_SNAKE:
                y = generator.single(START_ROW + 5, START_ROW + BOX_HEIGHT - 5);
                x = generator.single(START_COL + 5, START_COL + BOX_WIDTH - 5);
                break;
            case GENERATE:
                y = generator.single(START_ROW, START_ROW + BOX_HEIGHT - 1);
                x = generator.single(START_COL, START_COL + BOX_WIDTH);
                break;
            case GENERATE_COOR:
                y = generator.single(START_ROW, START_ROW + BOX_HEIGHT);
                x = generator.single(START_COL, START_COL + BOX_WIDTH);
            default:
                // just for debug
                throw std::runtime_error("Invalid generateTag");
//                std::unreachable();
        }
    }
    virtual ~Point() noexcept = default;
public:
    NODISCARD virtual constexpr inline short getY() const noexcept final {
        return y;
    }

    NODISCARD virtual constexpr inline short getX() const noexcept final {
        return x;
    }

    NODISCARD virtual constexpr inline char getChar() const noexcept final {
        return c;
    }

    NODISCARD virtual constexpr direction getDirection() const noexcept final {
        return nDirection;
    }

    NODISCARD virtual constexpr unsigned short count() const noexcept = 0;

    virtual Point& show() noexcept = 0;
public:
    short y;
    short x;
    char c;
    direction nDirection;
};