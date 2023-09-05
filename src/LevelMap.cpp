#include <algorithm>
#include "LevelMap.h"
#include <SDL.h>
#include "Common.h"
#include "Utils.h"

LevelMap::LevelMap()
: _level_size_x(0)
, _level_size_y(0)
{
	new_game();
}

void LevelMap::new_game()
{
	_field.clear();
	_balls.clear();
	generate();
	_blocks_collider = spLevelMapCollider(new LevelMapCollider(this));
	_player = spPlayer(new Player(&getCollider()));
	_balls.emplace_back(new Ball(1));
	_player->setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT });
}

void LevelMap::update(float dt)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT])
		_joystick.setDirection(-1.f, 0.f);
	else if (state[SDL_SCANCODE_RIGHT])
		_joystick.setDirection(1.f, 0.f);
	else
		_joystick.setDirection(0.f, 0.f);

	updateBalls(dt);

	// Update player
	if (_player)
		if (_joystick.getSquaredForce() > 0.f)
			_player->move(_joystick.getDirection(), dt);
}

void LevelMap::onMouseMove(SDL_MouseMotionEvent* event)
{
	_player->setX(float(event->x));
}

void LevelMap::destroyBlock(const math::point2& block_field_position)
{
	FieldBlock& field_block = _field[std::size_t(block_field_position.y)][std::size_t(block_field_position.x)];
	FieldBlock* field_block_ptr = &field_block;
	destroyBlock(field_block_ptr);
}

void LevelMap::destroyBlock(FieldBlock* block_ptr)
{
	*block_ptr = BLOCK_NONE;
	_blocks_collider->removeBlockCollider(block_ptr);
}

int LevelMap::damageBlock(FieldBlock* block_ptr, int dmg)
{
	if (dmg < 1)
		return 0;
	if (*block_ptr == BLOCK_BRICK)
	{
		destroyBlock(block_ptr);
		return dmg - 1;
	}
	else if (*block_ptr == BLOCK_CONCRETE)
	{
		if (dmg == 1)
		{
			*block_ptr = BLOCK_BRICK;
			return 0;
		}
		else
		{
			destroyBlock(block_ptr);
			return dmg - 2;
		}
		return dmg - 2;
	}
	return 0;
}

void LevelMap::onMainAction()
{
	// Update first ball in initial state
	if (!_balls.empty())
	{
		auto&& ball = _balls.front();
		if (!ball->running())
			ball->kickUp();
	}
}

void LevelMap::onMouseButtonDown()
{
	onMainAction();
}

void LevelMap::onMouseButtonUp()
{
}

bool LevelMap::updateBalls(float dt)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_SPACE])
		onMainAction();

	// Update all balls movement
	for (auto&& ball_ptr : _balls)
	{
		auto& ball = *ball_ptr;
		if (!ball.running())
		{
			auto& pc = _player->getCollider();
			auto p = _player->getPosition();
			p.y -= pc.getSize().y;
			//p.y += ball.getCollider().getSize() / 2.f;
			ball.setPosition(p);
		}
		else
		{
			ball.update(dt);
			if (ball.getPosition().y > SCREEN_HEIGHT)
			{
				new_game();
				return false;
			}
			auto it = _blocks_collider->check(ball.getCollider());
			if (it != _blocks_collider->nullBlockIt())
			{
				auto block_ptr = it->first;
				auto col = it->second;
				auto dmg_remaining = damageBlock((FieldBlock*)it->first, ball.getPower());
				if (dmg_remaining == 0)
					ball.bounce(col);
				ball.setPower(dmg_remaining);
			}
			else
			{
				if (_player->getCollider().check(ball.getCollider()))
					ball.bounce(_player->getCollider(), true);
			}
		}
	}
	if (_blocks_collider->empty())
	{
		auto power = _balls.front()->getInitialPower();
		new_game();
		_balls.front()->setInitialPower(power + 1);
		_balls.front()->setPower(power + 1);
	}
	return true;
}

void LevelMap::render(SDL_Renderer* renderer)
{
	for(int y = 0; y < _level_size_y; y++)
	{
		const std::vector<FieldBlock>& field_row = _field[y];
		for(int x = 0; x < _level_size_x; x++)
		{
			const FieldBlock& block = field_row[x];
			SDL_Rect block_rect;
			block_rect.x = x * brick_width;
			block_rect.y = y * brick_height;
			block_rect.h = brick_height;
			block_rect.w = brick_width;
			uint8_t r = 0, g = 0, b = 0;
			switch(block)
			{
				case BLOCK_CONCRETE:
					r = 164;
					g = 165;
					b = 164;
					break;
				case BLOCK_BRICK:
					r = 200;
					g = 100;
					b = 0;
					break;
				case BLOCK_NONE:
					break;
				default:
					break;
			}
			if(block != BLOCK_NONE)
			{
				SDL_SetRenderDrawColor(renderer, r, g, b, 0);
				SDL_RenderFillRect(renderer, &block_rect);
			}
		}
	}
	_player->render(renderer);
	for (auto& ball : _balls)
		ball->render(renderer);
}

void LevelMap::generate()
{
	_level_size_x = int(SCREEN_WIDTH / brick_width);
	_level_size_y = int(SCREEN_HEIGHT / 2 / brick_height);
	_field.reserve(_level_size_y);
	for(int y = 0; y < _level_size_y; y++)
	{
		std::vector<FieldBlock> field_row(_level_size_x, BLOCK_NONE);
		field_row.reserve(_level_size_x);
		for(int x = 0; x < _level_size_x; x++)
		{
			if(x == 0 || x == _level_size_x - 1 || y == 0 || y == _level_size_y - 1 || (y % 2 == 0 && x % 2 == 0))
			{
				field_row[x] = BLOCK_CONCRETE;
			}
			else if(!(x == 1 && y == 1) && !(x == 1 && y == 2) && ! (x == 2 && y == 1)) // start position should be clean
			{
				field_row[x] = (FieldBlock)Utils::random(0, 2) == 0 ? BLOCK_BRICK : BLOCK_NONE;
			}
			else
			{
				field_row[x] = BLOCK_NONE;
			}
		}
		_field.push_back(field_row);
	}
}
