#include <stdio.h>
#include "Joystick.h"

void Joystick::setPosition(float x, float y)
{
	_origin = {x, y};
}

void Joystick::setDirection(float x, float y)
{
	_pointer = {x, y};
	_direction = _pointer - _origin;
}

void Joystick::reset()
{
	_direction = {0, 0};
	_origin = {0, 0};
}

const math::vector2& Joystick::getDirection()
{
	return _direction;
}
float Joystick::getSquaredForce()
{
	return _direction.sqlength();
}
