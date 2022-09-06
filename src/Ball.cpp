#include <cmath>
#include "Utils.h"
#include "vector_2d.h"
#include "Ball.h"

#define MIN_VX 10.f

Ball::Ball(int power)
: _power(power)
, _initial_power(power)
, _collider(_size.x)
{
}

void Ball::bounce(const BoxCollider& collider, bool controllable)
{
	auto&& p = _collider.getPosition();
	auto&& s = _collider.getSize();
	auto&& p2 = collider.getPosition();
	auto&& s2 = collider.getSize();

	if (p.y < p2.y && _velocity.y > 0.f)
		_velocity.y *= -1.f;
	else if (p.y > p2.y && _velocity.y < 0.f)
		_velocity.y *= -1.f;

	if (std::fabs(_velocity.x) < MIN_VX || controllable)
	{
		auto dp = p - p2;
		_velocity.x = dp.x * 10;
	}
}

void Ball::update(float dt)
{
	// Update position
	math::vector2 ds = _velocity * dt;
	math::vector2 current_position = getPosition();
	math::vector2 p = current_position + ds;
	setPosition(p);

	// Update direction
	if (p.x < 0.f && _velocity.x < 0.f)
		_velocity.x *= -1.f;
	else if (p.x > float(SCREEN_WIDTH) && _velocity.x > 0.f)
		_velocity.x *= -1.f;

	if (p.y < 0.f && _velocity.y < 0.f)
		_velocity.y *= -1.f;
	else if (p.y > float(SCREEN_WIDTH) && _velocity.y > 0.f)
		_velocity.y *= -1.f;
}

void Ball::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
    SDL_Rect ball_rect;
	auto& p = getPosition();
    ball_rect.x = Utils::clamp(p.x - _size.x / 2, 0.f, float(SCREEN_WIDTH));
    ball_rect.y = Utils::clamp(p.y - _size.y / 2, 0.f, float(SCREEN_HEIGHT));
    ball_rect.w = _size.x;
    ball_rect.h = _size.y;
    //RenderFillCircle(buf, p, ball_rect.w / 2.f);
	SDL_RenderFillRect(renderer, &ball_rect);
}

