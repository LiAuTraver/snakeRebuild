module;
#include "core.hpp"
export module ancillarycat.entities:food;
import :entity;
export class Food;

class Food final : public Entity {
public:
	// as now food is simple, I don't think it's necessary to have lots of ctor
	explicit Food(const char& _c) : Entity(GENERATE, _c) {
		Entity::show();
	}
	virtual ~Food() noexcept override {}
};
