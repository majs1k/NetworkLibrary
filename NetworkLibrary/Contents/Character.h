#pragma once
#include <string>

struct Character
{
	int inventoryId_;
	int characterId_;

	int level_;
	int attack_;
	int hp_;
	int currentHp_;

	void Initialize(int inventoryId, int characterId, int level, int attack, int hp)
	{
		inventoryId_ = inventoryId;
		characterId_ = characterId;
		level_ = level;
		attack_ = attack;
		hp_ = hp;
	}
};
