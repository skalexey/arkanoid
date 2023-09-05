#pragma once

#include <memory>
#include <SDL.h>
#include "vector_2d.h"
#include "BoxCollider.h"
#include "Common.h"

class LevelMapCollider;

class Player
{
public:
	Player(const LevelMapCollider* collider);
	void render(SDL_Renderer* renderer) const;
	void setPosition(const math::vector2& pos);
	void setX(float x);
	void setY(float y);
	const math::vector2& getPosition() const {
		return _collider.getPosition();
	}
	BoxCollider& getCollider() {
		return _collider;
	}
	void move(const math::vector2& direction, float dt);

private:
	const math::vector2 _size = { 160.f, 35.f };
	const float _default_speed = 1000.f;
	BoxCollider _collider;
	const LevelMapCollider* _level_map_collider;
};
using spPlayer = std::shared_ptr<Player>;
