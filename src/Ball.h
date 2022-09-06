#pragma once

#include <functional>
#include <memory>
#include <SDL.h>
#include "vector_2d.h"
#include "Common.h"
#include "CircleCollider.h"
#include "BoxCollider.h"

class Ball
{
public:
	Ball(int power);
    void render(SDL_Renderer* renderer) const;
	void setPosition(const math::vector2& position) {
		_collider.setPosition(position);
	}
	const math::vector2& getPosition() const {
		return _collider.getPosition();
	}
	void setOnAffect(const std::function<void()>& on_affect)	{
		_on_affect = on_affect;
	}
	int getPower() const	{
		return _power;
	}
	int getInitialPower() const {
		return _initial_power;
	}
	void setPower(int power) {
		_power = power > 0 ? power : _initial_power;
	}
	void setInitialPower(int power) {
		_initial_power = power;
	}
	const CircleCollider& getCollider() const {
		return _collider;
	}
	void bounce(const BoxCollider& collider, bool controllable = false);
	void kickUp() {
		_velocity = { 0, -500.f };
	}
	bool running() const {
		return _velocity.sqlength() != 0.f;
	}
	void update(float dt);

private:
    int _power = 0;
	int _initial_power = 0;
	const math::vector2 _size = { 20.f, 20.f };
	CircleCollider _collider;
	math::vector2 _velocity = {0.f, 0.f};

private:
    std::function<void()> _on_affect;
};
using spBall = std::shared_ptr<Ball>;

