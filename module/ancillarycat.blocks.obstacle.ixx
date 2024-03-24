module;

#include "../include/config.hpp"
#include <Windows.h>

export module ancillarycat.blocks:obstacle;

import :block;
import ancillarycat.api;
import ancillarycat.console;
import std;

export class Obstacle final : public Block {
public:
	Obstacle() : Block(GENERATE_COOR, ' ') { fillColor = ansiBackground::whiteIntense; }
	explicit Obstacle(const char& _c) : Block(GENERATE_COOR, _c) { fillColor = ansiBackground::whiteIntense; countObstacle_++;}
	explicit Obstacle(const short& _y, const short& _x, const char& _c) : Block(_y, _x, _c) { fillColor = ansiBackground::blueIntense; countObstacle_++;}
	virtual ~Obstacle() override {
		console
			.setCursorCoordinate(this->y, this->x)
			.print(" ", ansiColor::reset);
		countObstacle_--;
	}
	virtual Obstacle& show() noexcept override {
		console
			.setCursorCoordinate(this->y, this->x)
			.print("{}", ansiColor::whiteIntense, fillColor, c);
		return *this;
	}
    [[nodiscard]] virtual constexpr unsigned short count() const noexcept override {
        return countObstacle_;
    }
public:
	ansiBackground fillColor;
private:
	static short countObstacle_;
};

short Obstacle::countObstacle_ = 0;