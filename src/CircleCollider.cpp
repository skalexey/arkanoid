//
//  CircleCollider.cpp
#include "CircleCollider.h"
#include "LevelMapCollider.h"

CircleCollider::CircleCollider(float _size)
: Collider()
, _size(_size)
, _size_squared(_size * _size)
{
	
}

float CircleCollider::getSize() const
{
	return _size;
}

bool CircleCollider::containPoint(int x, int y) const
{
	math::vector2 point_position(x, y);
	math::vector2 distance = point_position - _position;
	return distance.sqlength() <= _size_squared;
}

bool CircleCollider::check(const CircleCollider& collider2) const
{
	math::vector2 distance = collider2.getPosition() - _position;
	float collider2_size = collider2.getSize();
	return distance.sqlength() <= (collider2_size + _size) * (collider2_size + _size);
}

bool CircleCollider::check(const LevelMapCollider& collider2) const
{
	return collider2.check(*this) != collider2.nullBlockIt();
}
