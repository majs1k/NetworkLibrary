#include <process.h>
#include "FighterServer.h"
#include "Player.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <windows.h>

FighterServer::FighterServer()
{
	proxy_.server_ = this;

	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	// 종료시
	//shutdown_ = true;
}

bool FighterServer::OnConnectionRequest(const std::wstring& ip, int port)
{
	if (SessionCount() >= SessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void FighterServer::OnAccept(__int64 sessionId)
{
	this->CreatePlayer(sessionId);
}

void FighterServer::OnRelease(__int64 sessionId)
{
	this->RemovePlayer(sessionId);

	for (auto& p : playerMap_)
	{
		Player* player = p.second;

		//if (player->sessionId_ == sessionId)
		//	continue;

		proxy_.sc_character_delete(player->sessionId_, sessionId);
	}
}

void FighterServer::OnRecv(__int64 sessionId, Packet* packet)
{
	unsigned char type;
	*packet >> type;

	// 함수의 인자 자료형 주의 필요 (이후 패킷헤더 수정시 참고)
	if (!packetProc(sessionId, *packet, type))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}

void FighterServer::OnError(int errorCode, wchar_t* str)
{

}

unsigned int __stdcall FighterServer::LogicThread(void* param)
{
	FighterServer* server = (FighterServer*)param;

	while (!server->shutdown_)
	{
		server->Update();

		TickController::Instance().Update();
	}

	return 0;
}

void FighterServer::Update()
{
	for (auto& p : playerMap_)
	{
		Player* player = p.second;

		if (player->action_ == MOVE_DIR_NONE)
			continue;

		player->Move();
	}
}

void FighterServer::CreatePlayer(__int64 sessionId)
{
	// db 구현시 플레이어 id 조회
	Player* player = new Player();
	player->Initialize(sessionId);

	playerMap_.insert({ sessionId, player });
	playerCount_++;

	int playerId = player->playerId_;
	char direction = player->direction_;
	short x = static_cast<short>(player->x_);
	short y = static_cast<short>(player->y_);
	char hp = player->hp_;

	// 내 캐릭터 정보 나에게
	proxy_.sc_create_my_character(sessionId, playerId, direction, x, y, hp);

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		if (other->playerId_ == playerId)
			continue;

		// 내 캐릭터 정보 남에게
		proxy_.sc_create_other_character(other->sessionId_, playerId, direction, x, y, hp);

		// 남 캐릭터 정보 나에게
		proxy_.sc_create_other_character(sessionId, other->playerId_, other->direction_,
			static_cast<short>(other->x_), static_cast<short>(other->y_), other->hp_);

		if (other->action_ == MOVE_DIR_NONE)
			continue;

		// 남 캐릭터 이동중이면 나에게
		proxy_.sc_start_move(sessionId, other->playerId_, other->action_,
			static_cast<short>(other->x_), static_cast<short>(other->y_));
	}
}

void FighterServer::RemovePlayer(__int64 sessionId)
{
	auto it = playerMap_.find(sessionId);

	if (it == playerMap_.end())
		return;

	Player* player = (*it).second;

	playerMap_.erase(sessionId);

	delete player;

	playerCount_--;
}

bool FighterServer::cs_start_move(__int64 sessionId, char direction, short x, short y)
{
	auto it = playerMap_.find(sessionId);

	if (it == playerMap_.end())
		return false;

	Player* player = (*it).second;

	// 이동 오류 체크
	int deltaX = abs(x - player->x_);
	int deltaY = abs(y - player->y_);

	if (ERROR_RANGE < deltaX || ERROR_RANGE < deltaY)
	{
		LOG(L"[Network] invalid coord session=%d", sessionId);

		__debugbreak();
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

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		proxy_.sc_start_move(other->sessionId_, player->playerId_, player->action_,
			static_cast<short>(player->x_), static_cast<short>(player->y_));
	}

	return true;
}

bool FighterServer::cs_stop_move(__int64 sessionId, char action, short x, short y)
{
	auto it = playerMap_.find(sessionId);

	if (it == playerMap_.end())
		return false;

	Player* player = (*it).second;

	// 이동 오류 체크
	int deltaX = abs(x - player->x_);
	int deltaY = abs(y - player->y_);

	if (ERROR_RANGE < deltaX || ERROR_RANGE < deltaY)
	{
		LOG(L"[Network] invalid coord session=%d", sessionId);

		__debugbreak();

		return false;
	}

	player->action_ = MOVE_DIR_NONE;
	player->direction_ = action;
	player->x_ = x;
	player->y_ = y;

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		proxy_.sc_stop_move(other->sessionId_, player->playerId_, player->direction_,
			static_cast<short>(player->x_), static_cast<short>(player->y_));
	}

	return true;
}

bool FighterServer::cs_attack1(__int64 sessionId, char direction, short x, short y)
{
	auto it = playerMap_.find(sessionId);

	if (it == playerMap_.end())
		return false;

	Player* player = (*it).second;

	//player->direction_ = direction;
	//player->x_ = x;
	//player->y_ = y;

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		if (other == player)
			continue;

		// 단순히 공격 이펙트만 전송
		proxy_.sc_attack1(other->sessionId_, player->playerId_, player->direction_, player->x_, player->y_);
	}

	Player* target = nullptr;
	int deltaXMin = ATTACK1_RANGE_X;

	for (auto& p : playerMap_)
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
			if (player->x_ < other->x_ || deltaXMin <= deltaX)
				continue;

			deltaXMin = deltaX;
			target = other;
		}
	}

	if (!target)
		return true;

	// TODO: 최솟값 0
	target->hp_ -= ATTACK1_DAMAGE;

	for (auto& p : playerMap_)
	{
		Player* reciever = p.second;

		proxy_.sc_damage(reciever->sessionId_, player->playerId_, target->playerId_, target->hp_);
	}

	if (target->hp_ <= 0)
	{
		this->Disconnect(target->sessionId_);

		return true;
	}

	return true;
}
