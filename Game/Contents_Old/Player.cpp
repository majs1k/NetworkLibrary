#include "Player.h"
#include "../Utils/Random.h"
#include "../Utils/TickController.h"
#include "../Utils/Logger.h"

void Player::Initialize(__int64 sessionId)
{
	playerId_ = sessionId;
	sessionId_ = sessionId;

	action_ = MOVE_DIR_NONE;
	direction_ = MOVE_DIR_LL;

	x_ = RAND(RANGE_MOVE_LEFT, RANGE_MOVE_RIGHT);
	y_ = RAND(RANGE_MOVE_TOP, RANGE_MOVE_BOTTOM);
	//x_ = 0;
	//y_ = 0;
	hp_ = MAX_HP;
}

void Player::Move()
{
	switch (action_)
	{
	case MOVE_DIR_LL:
		this->MoveLeft();
		break;

	case MOVE_DIR_LU:
		this->MoveLeft();
		this->MoveUp();
		break;

	case MOVE_DIR_UU:
		this->MoveUp();
		break;

	case MOVE_DIR_RU:
		this->MoveRight();
		this->MoveUp();
		break;

	case MOVE_DIR_RR:
		this->MoveRight();
		break;

	case MOVE_DIR_RD:
		this->MoveRight();
		this->MoveDown();
		break;

	case MOVE_DIR_DD:
		this->MoveDown();
		break;

	case MOVE_DIR_LD:
		this->MoveLeft();
		this->MoveDown();
		break;
	}

	//LOG_INFO(L"[LOGIC_MOVE] ID: %d / Action: %d / X: %f / Y: %f", (int)sessionId_, (int)action_, x_, y_);
}

void Player::MoveLeft()
{
	float diffX = velocityX * TickController::Instance().DeltaTime() / CLOCKS_PER_SEC;

	if (RANGE_MOVE_LEFT + diffX <= x_)
	{
		x_ -= diffX;
		return;
	}

	x_ = RANGE_MOVE_LEFT;
	// 화면 끝에 닿을시 정지
	action_ = MOVE_DIR_NONE;
}

void Player::MoveRight()
{
	float diffX = velocityX * TickController::Instance().DeltaTime() / CLOCKS_PER_SEC;

	if (x_ + diffX <= RANGE_MOVE_RIGHT)
	{
		x_ += diffX;
		return;
	}

	x_ = RANGE_MOVE_RIGHT;
	action_ = MOVE_DIR_NONE;
}

void Player::MoveUp()
{
	float diffY = velocityY * TickController::Instance().DeltaTime() / CLOCKS_PER_SEC;

	if (RANGE_MOVE_TOP <= y_ - diffY)
	{
		y_ -= diffY;
		return;
	}

	y_ = RANGE_MOVE_TOP;
	action_ = MOVE_DIR_NONE;
}

void Player::MoveDown()
{
	float diffY = velocityY * TickController::Instance().DeltaTime() / CLOCKS_PER_SEC;

	if (y_ + diffY <= RANGE_MOVE_BOTTOM)
	{
		y_ += diffY;
		return;
	}

	y_ = RANGE_MOVE_BOTTOM;
	action_ = MOVE_DIR_NONE;
}
