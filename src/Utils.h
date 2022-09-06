#pragma once

namespace Utils
{
    int random(int from, int to);
	template <typename T>
	const T clamp(const T& v, const T& lo, const T& hi) {
		return v < lo ? lo : hi < v ? hi : v;
	}
}
