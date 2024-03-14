module;
#include "config.hpp"
#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif
export module ancillarycat.generator;
import std;

export class Generator {
public:
	_NODISCARD static direction direct() noexcept
	{
		return static_cast<direction>(random<int>(1, 4));
	}
	_NODISCARD static std::pair<short, short> coordinate(const short& y_lower_bound, const short& y_upper_bound,
		const short& x_lower_bound,
		const short& x_upper_bound) noexcept
	{
		return std::make_pair(random<short>(y_lower_bound, y_upper_bound), random<short>(x_lower_bound, x_upper_bound));
	}
	template <typename _T>
	_NODISCARD static _T random(const _T& lower_bound, const _T& upper_bound) noexcept
	{
		std::uniform_int_distribution<_T> distribution(lower_bound, upper_bound);
		return distribution(device_);
	}

	_NODISCARD static short single(const short& lower_bound, const short& upper_bound) noexcept
	{
		return random<short>(lower_bound, upper_bound);
	}

	_NODISCARD static std::chrono::milliseconds time(const short& lower_bound = 5000,
		const short& upper_bound = 15000) noexcept
	{
		return std::chrono::milliseconds(random<long long>(lower_bound, upper_bound));
	}

private:
	static std::random_device device_;
}generator;

export std::random_device Generator::device_;
