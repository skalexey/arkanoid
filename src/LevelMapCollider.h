#pragma once

#include <memory>
#include <unordered_map>
#include "Collider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "FieldBlock.h"
#include "rect.h"

class LevelMap;

class LevelMapCollider : public Collider
{
public:
    LevelMapCollider(LevelMap const* level_map, FieldBlock block_type = BLOCK_NONE);
	auto check(const CircleCollider& collider2) const {
		for (auto&& it = _colliders.begin(); it != _colliders.end(); ++it)
			if (it->second.check(collider2))
				return it;
		return _colliders.end();
	}
	auto check(const BoxCollider& collider2) const {
		for (auto&& it = _colliders.begin(); it != _colliders.end(); ++it)
			if (it->second.check(collider2))
				return it;
		return _colliders.end();
	}
    void removeBlockCollider(FieldBlock* block);
	auto findBlock(FieldBlock* block_ptr) const {
		auto it = _colliders.find(block_ptr);
		if (it != _colliders.end())
			return it;
		else
			return _colliders.end();
	}
	auto nullBlockIt() const {
		return _colliders.end();
	}

	bool empty() const {
		return _colliders.empty();
	}

private:
    void init(LevelMap const* level_map, FieldBlock block_type);
    LevelMap const* _level_map;
    std::unordered_map<const FieldBlock*, BoxCollider> _colliders;
};
using spLevelMapCollider = std::shared_ptr<LevelMapCollider>;
