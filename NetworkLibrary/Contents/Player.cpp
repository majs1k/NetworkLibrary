#include "Player.h"
#include "../Utils/Random.h"
#include "../Utils/TickController.h"
#include "../Utils/Logger.h"

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

	//LOG_INFO(L"[LOGIC_MOVE] ID: %d / Action: %d / X: %f / Y: %f", (int)sessionId_, (int)action_, x_, y_);
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
