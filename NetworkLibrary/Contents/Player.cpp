#include "Player.h"
#include "FighterServer.h"
//#include "../RPC/RPCProxy.h"
#include "../Utils/Random.h"
#include "../Utils/TickController.h"

void Player::initialize(__int64 sessionId)
{
	// TODO: db 연동 이후 수정
	playerId_ = sessionId;
	sessionId_ = sessionId;

	action_ = MOVE_DIR_NONE;
	direction_ = MOVE_DIR_LL;

	x_ = RAND(RANGE_MOVE_LEFT, RANGE_MOVE_RIGHT);
	y_ = RAND(RANGE_MOVE_TOP, RANGE_MOVE_BOTTOM);
	hp_ = MAX_HP;
}

int Player::playerId() const
{
	return playerId_;
}

__int64 Player::sessionId() const
{
	return sessionId_;
}

int Player::action() const
{
	return action_;
}

int Player::direction() const
{
	return direction_;
}

float Player::x() const
{
	return x_;
}

float Player::y() const
{
	return y_;
}

char Player::hp() const
{
	return hp_;
}

void Player::move()
{
	switch (action_)
	{
	case MOVE_DIR_LL:
		this->moveLeft();
		break;

	case MOVE_DIR_LU:
		this->moveLeft();
		this->moveUp();
		break;

	case MOVE_DIR_UU:
		this->moveUp();
		break;

	case MOVE_DIR_RU:
		this->moveRight();
		this->moveUp();
		break;

	case MOVE_DIR_RR:
		this->moveRight();
		break;

	case MOVE_DIR_RD:
		this->moveRight();
		this->moveDown();
		break;

	case MOVE_DIR_DD:
		this->moveDown();
		break;

	case MOVE_DIR_LD:
		this->moveLeft();
		this->moveDown();
		break;
	}

	//Log(L"# LOGIC_MOVE # ID: %d / Action: %d / X: %f / Y: %f", id_, action_, x_, y_);
}

void Player::moveLeft()
{
	float diffX = velocityX * TickController::getInstance().deltaTime() / CLOCKS_PER_SEC;

	if (RANGE_MOVE_LEFT + diffX <= x_)
	{
		x_ -= diffX;
		return;
	}

	x_ = RANGE_MOVE_LEFT;
	// 화면 끝에 닿을시 정지
	action_ = MOVE_DIR_NONE;
}

void Player::moveRight()
{
	float diffX = velocityX * TickController::getInstance().deltaTime() / CLOCKS_PER_SEC;

	if (x_ + diffX <= RANGE_MOVE_RIGHT)
	{
		x_ += diffX;
		return;
	}

	x_ = RANGE_MOVE_RIGHT;
	action_ = MOVE_DIR_NONE;
}

void Player::moveUp()
{
	float diffY = velocityY * TickController::getInstance().deltaTime() / CLOCKS_PER_SEC;

	if (RANGE_MOVE_TOP <= y_ - diffY)
	{
		y_ -= diffY;
		return;
	}

	y_ = RANGE_MOVE_TOP;
	action_ = MOVE_DIR_NONE;
}

void Player::moveDown()
{
	float diffY = velocityY * TickController::getInstance().deltaTime() / CLOCKS_PER_SEC;

	if (y_ + diffY <= RANGE_MOVE_BOTTOM)
	{
		y_ += diffY;
		return;
	}

	y_ = RANGE_MOVE_BOTTOM;
	action_ = MOVE_DIR_NONE;
}

void PlayerManager::createPlayer(__int64 sessionId)
{
	// db 구현후 플레이어 id는 조회 필요
	Player* player = new Player();
	player->initialize(sessionId);

	/// TODO: Lock 필요
	playerMap_.insert({ sessionId, player });
	playerCount_++;

	int playerId = player->playerId();
	int direction = player->direction();
	short x = static_cast<short>(player->x());
	short y = static_cast<short>(player->y());
	char hp = player->hp();

	// 내 캐릭터 정보 나에게
	proxy.sc_create_my_character(sessionId, playerId, direction, x, y, hp);

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		if (other->playerId() == playerId)
			continue;

		// 내 캐릭터 정보 남에게
		proxy.sc_create_other_character(other->sessionId(), playerId, direction, x, y, hp);

		// 남 캐릭터 정보 나에게
		proxy.sc_create_other_character(sessionId, other->playerId(), other->direction(), (short)other->x(), (short)other->y(), other->hp());

		if (other->action() == MOVE_DIR_NONE)
			continue;

		// 남 캐릭터 이동중이면 나에게
		proxy.sc_start_move(sessionId, other->playerId(), other->action(), (short)other->x(), (short)other->y());
	}
}

void PlayerManager::removePlayer(__int64 sessionId)
{
	auto it = playerMap_.find(sessionId);

	if (it == playerMap_.end())
		return;

	Player* player = (*it).second;

	/// TODO: Lock 필요
	playerMap_.erase(sessionId);

	delete player;

	playerCount_--;
}

void PlayerManager::update()
{
	for (auto& p : playerMap_)
	{
		Player* player = p.second;

		if (player->action() == MOVE_DIR_NONE)
			continue;

		player->move();
	}
}
