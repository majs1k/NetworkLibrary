#include "Player.h"
#include "../Network/Session.h"
#include "../Network/SessionManager.h"
#include "../RPC/RPCProxy.h"
#include "../Utils/Random.h"
#include "../Utils/TickController.h"

void Player::initialize(Session* session)
{
	// TODO: db 연동 이후 수정
	playerId_ = session->id();
	sessionId_ = session->id();

	action_ = MOVE_DIR_NONE;
	direction_ = MOVE_DIR_LL;

	x_ = RAND(RANGE_MOVE_LEFT, RANGE_MOVE_RIGHT);
	y_ = RAND(RANGE_MOVE_TOP, RANGE_MOVE_BOTTOM);
	hp_ = MAX_HP;

	isActive_ = true;
}

int Player::id() const
{
	return playerId_;
}

int Player::sessionId() const
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

bool Player::isActive() const
{
	return isActive_;
}

void Player::deactivate()
{
	isActive_ = false;
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

void PlayerManager::createPlayer(Session* session)
{
	// db 구현후 플레이어 id는 조회 필요
	Player* player = new Player();
	player->initialize(session);
	playerList_.push_back(player);
	size_++;

	int id = player->id();
	int direction = player->direction();
	short x = static_cast<short>(player->x());
	short y = static_cast<short>(player->y());
	char hp = player->hp();

	// 내 캐릭터 정보 나에게
	RPCProxy::sc_create_my_character(session, id, direction, x, y, hp);

	for (auto& other : playerList_)
	{
		if (other->id() == id)
			continue;

		Session* otherSession = SessionManager::getInstance().find(other->sessionId());
		if (otherSession == nullptr)
			continue;

		// 내 캐릭터 정보 남에게
		RPCProxy::sc_create_other_character(otherSession, id, direction, x, y, hp);

		// 남 캐릭터 정보 나에게
		RPCProxy::sc_create_other_character(session, other->id(), other->direction(), (short)other->x(), (short)other->y(), other->hp());

		if (other->action() == MOVE_DIR_NONE)
			continue;

		// 남 캐릭터 이동중이면 나에게
		RPCProxy::sc_start_move(session, other->id(), other->action(), (short)other->x(), (short)other->y());
	}
}

void PlayerManager::remove(Player* player)
{
	player->deactivate();

	for (auto& other : playerList_)
	{
		if (other == player)
			continue;

		Session* otherSession = SessionManager::getInstance().find(other->sessionId());
		if (otherSession == nullptr)
			continue;

		RPCProxy::sc_character_delete(otherSession, player->id());
	}
}

void PlayerManager::lazyDeletion()
{
	for (auto it = playerList_.begin(); it != playerList_.end(); )
	{
		Player* player = *it;

		if (!player->isActive())
		{
			delete player;
			size_--;

			it = playerList_.erase(it);
		}
		else
			++it;
	}
}

Player* PlayerManager::findBySessionId(int sessionId)
{
	for (auto& p : playerList_)
	{
		if (p->sessionId() == sessionId)
			return p;
	}

	return nullptr;
}

void PlayerManager::update()
{
	for (auto& player : playerList_)
	{
		// FIXME: 컨텐츠 분리 작업 수업 후 수정 요함
		Session* session = SessionManager::getInstance().find(player->sessionId());

		if (session == nullptr)
		{
			PlayerManager::getInstance().remove(player);
			continue;
		}

		if (player->action() == MOVE_DIR_NONE)
			continue;

		player->move();
	}

	this->lazyDeletion();
}
