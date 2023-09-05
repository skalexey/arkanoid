#pragma once

#include "Collider.h"

class CircleCollider;
class LevelMapCollider;

class BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(float x, float y, float w, float h);
	bool check(const CircleCollider& collider2) const;
	bool check(const BoxCollider& collider2) const;
	bool check(const LevelMapCollider& collider2) const;
	math::vector2 getSize() const;
	bool containPoint(int x, int y) const;

private:
	float _w;
	float _h;
};
