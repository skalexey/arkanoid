
#pragma once

#include <memory>
#include <set>
#include <vector>
#include <SDL.h>
#include "Ball.h"
#include "Player.h"
#include "FieldBlock.h"
#include "LevelMapCollider.h"
#include "vector_2d.h"
#include "Joystick.h"

const float brick_width = 80.f;
const float brick_height = 50.f;

enum CheckCollisionResult
{
	COLLISION_NONE,
	COLLISION_XY,
	COLLISION_X,
	COLLISION_Y
};

class LevelMapCollider;

using TLevelMapField = std::vector<std::vector<FieldBlock> >;

class LevelMap
{
public:
	LevelMap();
	void render(SDL_Renderer* renderer);
	const LevelMapCollider& getCollider() const {
		return *_blocks_collider;
	}
	const TLevelMapField& getField() const {
		return _field;
	}
	bool updateBalls(float dt);
	void destroyBlock(const math::point2& block_field_position);
	void destroyBlock(FieldBlock* block_ptr);
	// Return remaining damage
	int damageBlock(FieldBlock* block_ptr, int dmg);
	void update(float dt);
	void onMouseMove(SDL_MouseMotionEvent* event);
	void onMouseButtonDown();
	void onMouseButtonUp();
	void onMainAction();
		
private:
	void generate();
	void new_game();
	TLevelMapField _field;
	int _level_size_x = 0;
	int _level_size_y = 0;
	spLevelMapCollider _blocks_collider;
	spPlayer _player;
	using balls_t = std::vector<spBall>;
	balls_t _balls;
	Joystick _joystick;

};
using LevelMapPtr = std::unique_ptr<LevelMap>;
