#include "LevelMap.h"
#include "LevelMapCollider.h"

LevelMapCollider::LevelMapCollider(LevelMap const* level_map, FieldBlock block_type)
: Collider()
, _level_map(nullptr)
{
	init(level_map, block_type);
}

void LevelMapCollider::removeBlockCollider(FieldBlock* block)
{
	_colliders.erase(block);
}

void LevelMapCollider::init(LevelMap const* level_map, FieldBlock block_type)
{
	const TLevelMapField& field = level_map->getField();
	for(int y = 0; y < field.size(); y++)
	{
		const std::vector<FieldBlock>& row_in_map = field[y];
		for(int x = 0; x < row_in_map.size(); x++)
		{
			const FieldBlock& block = row_in_map[x];
			if((block_type == BLOCK_NONE && (block == BLOCK_BRICK || block == BLOCK_CONCRETE)) || (block_type == block && block_type != BLOCK_NONE))
			{
				_colliders[&block] = BoxCollider((x + 0.5f) * brick_width, (y + 0.5f) * brick_height, brick_width, brick_height);
			}
		}
	}
}


