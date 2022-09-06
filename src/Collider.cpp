#include "Collider.h"

Collider::Collider()
{
}

void Collider::setPosition(const math::vector2& new_position)
{
    _position = new_position;
}

bool Collider::containPoint(int x, int y) const
{
    return false;
}

const math::vector2& Collider::getPosition() const
{
    return _position;
}

