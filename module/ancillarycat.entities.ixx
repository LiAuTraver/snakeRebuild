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
		return std::move(std::make_unique<Star>());
	case 4: [[fallthrough]];
	case 5:
		return std::move(std::make_unique<Dollar>());
	case 6:
		return std::move(std::make_unique<Exclamation>());
	default:
		std::unreachable();
	}
	//std::unreachable();
}

