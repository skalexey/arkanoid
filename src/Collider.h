#pragma once

#include <vector>
#include "vector_2d.h"

class Collider
{
public:
    Collider();
    virtual bool containPoint(int x, int y) const;
    void setPosition(const math::vector2& new_position);
    const math::vector2& getPosition() const;

protected:
	math::vector2 _position;
};
