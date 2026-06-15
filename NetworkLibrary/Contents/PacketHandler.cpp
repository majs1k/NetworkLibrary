#include "PacketHandler.h"
#include "Player.h"
#include "../RPC/RPCProxy.h"
#include "../Utils/Logger.h"

bool PacketHandler::cs_start_move(__int64 sessionId, char direction, short x, short y)
{
	Player* player = PlayerManager::getInstance().findBySessionId(sessionId);
	if (player == nullptr)
		return false;

	// 이동 오류 체크
	int deltaX = abs(x - player->x_);
	int deltaY = abs(y - player->y_);

	if (ERROR_RANGE < deltaX || ERROR_RANGE < deltaY)
	{
		LOG(L"[Network] invalid coord session=%d", sessionId);

		return false;
	}

	player->action_ = direction;

	switch (direction)
	{
	case MOVE_DIR_LL:
	case MOVE_DIR_LU:
	case MOVE_DIR_LD:
		player->direction_ = MOVE_DIR_LL;
		break;

	case MOVE_DIR_RR:
	case MOVE_DIR_RU:
	case MOVE_DIR_RD:
		player->direction_ = MOVE_DIR_RR;
		break;
	}

	player->x_ = x;
	player->y_ = y;

	for (auto& p : PlayerManager::getInstance().playerList_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		RPCProxy::sc_start_move(other->sessionId_, player->playerId_, player->action_,
			static_cast<short>(player->x_), static_cast<short>(player->y_));
	}

	return true;
}

bool PacketHandler::cs_stop_move(__int64 sessionId, char action, short x, short y)
{
	Player* player = PlayerManager::getInstance().findBySessionId(sessionId);
	if (player == nullptr)
		return false;

	// 이동 오류 체크
	int deltaX = abs(x - player->x_);
	int deltaY = abs(y - player->y_);

	if (ERROR_RANGE < deltaX || ERROR_RANGE < deltaY)
	{
		LOG(L"[Network] invalid coord session=%d", sessionId);

		return false;
	}

	player->action_ = MOVE_DIR_NONE;
	player->direction_ = action;
	player->x_ = x;
	player->y_ = y;

	for (auto& p : PlayerManager::getInstance().playerList_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		RPCProxy::sc_stop_move(other->sessionId_, player->playerId_, player->direction_,
			static_cast<short>(player->x_), static_cast<short>(player->y_));
	}

	return true;
}

bool PacketHandler::cs_attack1(__int64 sessionId, char direction, short x, short y)
{
	Player* player = PlayerManager::getInstance().findBySessionId(sessionId);
	if (player == nullptr)
		return false;

	//player->direction_ = direction;
	//player->x_ = x;
	//player->y_ = y;

	for (auto& p : PlayerManager::getInstance().playerList_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		// 단순히 공격 이펙트만 전송
		RPCProxy::sc_attack1(other->sessionId_, player->playerId_, player->direction_, player->x_, player->y_);
	}

	Player* target = nullptr;
	int deltaXMin = ATTACK1_RANGE_X;

	for (auto& p : PlayerManager::getInstance().playerList_)
	{
		Player* other = p.second;

		if (player->playerId_ == other->playerId_)
			continue;

		int deltaY = abs(player->y_ - other->y_);

		if (ATTACK1_RANGE_Y < deltaY)
			continue;

		if (player->direction_ == MOVE_DIR_RR)
		{
			int deltaX = abs(other->x_ - player->x_);
			if (other->x_ < player->x_ || deltaXMin <= deltaX)
				continue;

			deltaXMin = deltaX;
			target = other;
		}
		else
		{
			int deltaX = abs(other->x_ - player->x_);
			if ( player->x_ <other->x_  || deltaXMin <= deltaX)
				continue;

			deltaXMin = deltaX;
			target = other;
		}
	}

	if (!target)
		return true;

	// TODO: 최솟값 0
	target->hp_ -= ATTACK1_DAMAGE;

	for (auto& p : PlayerManager::getInstance().playerList_)
	{
		Player* other = p.second;

		RPCProxy::sc_damage(other->sessionId_, player->playerId_, target->playerId_, target->hp_);
	}

	if (target->hp_ <= 0)
	{
		for (auto& p : PlayerManager::getInstance().playerList_)
		{
			Player* everyPlayer = p.second;

			RPCProxy::sc_character_delete(everyPlayer->sessionId_, target->playerId_);
		}

		/// HOW TO?
		//server->decrementIoCount(target->sessionId_);
		//PlayerManager::getInstance().removePlayer(target->playerId_);

		return true;
	}

	return true;
}
