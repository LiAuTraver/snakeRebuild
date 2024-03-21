module;
export module ancillarycat.entities;

import std;

export import :entity;
export import :snake;
export import :food;
import ancillarycat.api;

// uniform_int food generator
export std::unique_ptr<Food> generate_food() noexcept;

std::unique_ptr<Food> generate_food() noexcept {
	std::unique_ptr<Food> food;
	switch (generator.single(1, 6)) {
	case 1: [[fallthrough]];
	case 2: [[fallthrough]];
	case 3:
		food = std::make_unique<Star>();
		break;
	case 4: [[fallthrough]];
	case 5:
		food = std::make_unique<Dollar>();
		break;
	case 6:
		food = std::make_unique<Exclamation>();
		break;
	default:
		std::unreachable();
	}
	return std::move(food);
}

