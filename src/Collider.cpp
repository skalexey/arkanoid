#include "Collider.h"

Collider::Collider()
{
}

void Collider::setPosition(const math::vector2& new_position)
{
	_position = new_position;
}

void Collider::setX(float x)
{
	_position.x = x;
}

void Collider::setY(float y)
{
	_position.y = y;
}

bool Collider::containPoint(int x, int y) const
{
	return false;
}

const math::vector2& Collider::getPosition() const
{
	return _position;
}

