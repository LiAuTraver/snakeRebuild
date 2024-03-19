module;
export module ancillarycat.entities;

import std;

export import :entity;
export import :snake;
export import :food;
import ancillarycat.api;

// uniform_int food generator
export std::vector <Food> generate_food(const short &min, const short &max) noexcept;


std::vector <Food> generate_food(const short &min, const short &max) noexcept {
    std::vector <Food> foods;
    for (short i = 0; i < generator.single(min, max); i++) {
        switch (generator.single(1, 6)) {
            case 1:[[fallthrough]];
            case 2:[[fallthrough]];
            case 3:
                foods.emplace_back(Star());
                break;
            case 4:[[fallthrough]];
            case 5:
                foods.emplace_back(Dollar());
                break;
            case 6:
                foods.emplace_back(Exclamation());
                break;
            default:
                std::unreachable();
        }
    }
    return foods;
}
