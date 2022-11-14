#pragma once

#include <cmath>
#include <SDL.h>

void RenderFillCircle(SDL_Renderer* renderer, const math::vector2& pos, float r)
{
	const auto d = r * 2;
	const auto r2 = r * r;
	for (int i = -r; i <= r; i++)
		for (int g = -r; g <= r; g++)
			if (i * i + g * g <= r2)
				SDL_RenderDrawPoint(renderer, std::max(0, int(pos.x + i)), std::max(0, int(pos.y + g)));
}
