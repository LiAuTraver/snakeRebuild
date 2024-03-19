module;

#include "../include/config.hpp"

export module ancillarycat.entities:food;

import :entity;

export class Food;
export class Dollar;
export class Exclamation;
export class Star;

class Food : public Entity {
public:
    // as now food is simple, I don't think it's necessary to have lots of ctor
    explicit Food(const char &_c) : Entity(GENERATE, _c) {
        Entity::show();
        count++;
    }

    virtual ~Food() noexcept override {}

public:
    short weight = 0;
    static short count;
};

export short Food::count = 0;

class Dollar : public Food {
public:
    explicit Dollar() : Food('$') {
        count++;
    }

    virtual ~Dollar() noexcept override {}
public:
    short weight = 2;
    static short count;
};

export short Dollar::count = 0;

class Exclamation : public Food {
public:
    explicit Exclamation() : Food('!') {
        count++;
    }

    virtual ~Exclamation() noexcept override {}
public:
    short weight = 3;
    static short count;
};

export short Exclamation::count = 0;

class Star : public Food {
public:
    explicit Star() : Food('*') {
        count++;
    }
    virtual ~Star() noexcept override {}
public:
    short weight = 1;
    static short count;
};

export short Star::count = 0;