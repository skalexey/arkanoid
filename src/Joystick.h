#pragma once

#include "vector_2d.h"

class Joystick
{
public:
	void setPosition(float x, float y);
	void setDirection(float x, float y);
	void reset();
	const math::vector2& getDirection();
	float getSquaredForce();

private:
	math::vector2 _origin;
	math::vector2 _pointer;
	math::vector2 _direction;
};
