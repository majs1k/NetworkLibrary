#pragma once
#include <string>

//enum class PLAYER_STATE : short
//{
//	LOBBY,
//	INGAME,
//};

struct Character
{
	int inventoryId_;
	int characterId_;
	//int playerId_;
	
	int level_;
	int attack_;
	int hp_;
	//int statPoint_;

	void Initialize(int inventoryId, int characterId, int level, int attack, int hp)
	{
		inventoryId_ = inventoryId;
		characterId_ = characterId;
		level_ = level;
		attack_ = attack;
		hp_ = hp;
		//statPoint_ = statPoint;
	}
};
