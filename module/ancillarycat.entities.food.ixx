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
	explicit Food(const char& _c) : Entity(GENERATE, _c) {
		Entity::show();
		Food::countFood_++;
	}
	virtual ~Food() noexcept override
	{
		countFood_--;
	}
public:
	[[nodiscard]] virtual short weight() const noexcept final {
		return weight_;
	}
	[[nodiscard]] virtual constexpr unsigned short count() const noexcept override {
		return countFood_;
	}
protected:
	short weight_ = 0;
private:
	static unsigned short countFood_;
};

unsigned short Food::countFood_ = 0;

class Star final : public Food {
public:
	explicit Star() : Food('*') {
		countStar_++;
		weight_ = 1;
	}
	virtual ~Star() noexcept override
	{
		countStar_--;
	}
private:
	static unsigned short countStar_;
};

unsigned short Star::countStar_ = 0;

class Dollar final : public Food {
public:
	explicit Dollar() : Food('$') {
		countDollar_++;
		weight_ = 2;
	}
	virtual ~Dollar() noexcept override
	{
		countDollar_--;
	}
public:
	[[nodiscard]] virtual constexpr unsigned short count() const noexcept override {
		return countDollar_;
	}
private:
	static unsigned short countDollar_;
};

unsigned short Dollar::countDollar_ = 0;


class Exclamation final : public Food {
public:
	explicit Exclamation() : Food('!') {
		countExclamation_++;
		weight_ = 3;
	}
	virtual ~Exclamation() noexcept override
	{
		countExclamation_--;
	}
private:
	static unsigned short countExclamation_;
};

unsigned short Exclamation::countExclamation_ = 0;