#include <cmath>
#include <map>
#include "LevelMap.h"
#include "Utils.h"
#include "Player.h"

Player::Player(const LevelMapCollider* level_map_collider)
: _collider(0, 0, _size.x, _size.y)
, _level_map_collider(level_map_collider)
{
}

void Player::render(SDL_Renderer* renderer) const
{
    const math::vector2& player_position = getPosition();
	auto w = _size.x;
	auto h = _size.y;
	SDL_Rect player_rect{
		(int)Utils::clamp(player_position.x - w / 2.f, 0.f, float(SCREEN_WIDTH))
		, (int)Utils::clamp(player_position.y - h / 2, 0.f, float(SCREEN_HEIGHT))
		, (int)w
		, (int)h
	};
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 0);
    SDL_RenderFillRect(renderer, &player_rect);
}

void Player::setPosition(const math::vector2& pos)
{
	_collider.setPosition({
		Utils::clamp(pos.x, 0.f, float(SCREEN_WIDTH))
		, Utils::clamp(pos.y, 0.f, float(SCREEN_HEIGHT))
	});
}

void Player::move(const math::vector2& direction, float dt)
{
    math::vector2 p = direction;
    p *= (_default_speed * dt);
    setPosition(getPosition() + p);
}

