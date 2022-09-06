#pragma once

#include "Collider.h"

class LevelMapCollider;

class CircleCollider : public Collider
{
public:
    CircleCollider(float _size);
    bool check(const CircleCollider& collider2) const;
    bool check(const LevelMapCollider& collider2) const;
    float getSize() const;
    bool containPoint(int x, int y) const;
private:
    float _size;
    float _size_squared;
};
