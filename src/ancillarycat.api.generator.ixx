module;

#include "../include/config.hpp"

export module ancillarycat.api:generator;

import std;

export enum class direction : int {
	NO_DIRECTION = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

export class Generator {
public:
	template<typename _T>
	NODISCARD static _T uniform_int(const _T& lower_bound, const _T& upper_bound) noexcept {
		std::uniform_int_distribution <_T> distribution(lower_bound, upper_bound);
		return distribution(device_);
	}

	template<typename _T>
	NODISCARD static _T uniform_real(const _T& lower_bound, const _T& upper_bound) noexcept {
		std::uniform_real_distribution <_T> distribution(lower_bound, upper_bound);
		return distribution(device_);
	}

	NODISCARD static std::vector <std::vector<double>> heightMap(const short& rows, const short& cols) noexcept {
		auto heightMap =
			std::vector(rows, std::vector(cols, 0.0));
		for (auto r = 0; r < rows; ++r)
			for (auto c = 0; c < cols; ++c)
				heightMap[r][c] = uniform_real<double>(0.0, 1.0);
		return heightMap;
	}

	NODISCARD static std::pair<short, short>
		coordinate(const short& y_lower_bound,
			const short& y_upper_bound,
			const short& x_lower_bound,
			const short& x_upper_bound) noexcept {
		return std::make_pair(uniform_int<short>(y_lower_bound, y_upper_bound),
			uniform_int<short>(x_lower_bound, x_upper_bound));
	}

	NODISCARD static direction direct() noexcept {
		return static_cast<direction>(uniform_int<int>(1, 4));
	}

	NODISCARD static short single(const short& lower_bound, const short& upper_bound) noexcept {
		return uniform_int<short>(lower_bound, upper_bound);
	}

	NODISCARD static std::chrono::milliseconds time(const short& lower_bound = 5000,
		const short& upper_bound = 15000) noexcept {
		return std::chrono::milliseconds(uniform_int<long long>(lower_bound, upper_bound));
	}

private:
	static std::random_device device_;
} generator;

std::random_device Generator::device_;
