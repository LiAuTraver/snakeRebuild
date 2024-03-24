module;

#include "../include/config.hpp"

export module ancillarycat.blocks:block;

import std;
import ancillarycat.api;
import ancillarycat.console;

export class Block : public Point {
public:
//    Clang-Tidy: Use '= default' to define a trivial default constructor
//    explicit Block() : Point() {}
//    Block(const Block &block) : Point(block) {}
    explicit Block() = default;

    Block(const Block &block) = default;

    explicit Block(const short &_y, const short &_x, const char &_c) : Point(_y, _x, _c) {}

    explicit Block(const short &_y, const short &_x, const char &_c, const direction &_d) : Point(_y, _x, _c, _d) {}

    explicit Block(const int &generateTag, const char &_c) : Point(generateTag, _c) {}

    virtual ~Block() noexcept = default;

public:

};