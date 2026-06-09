#pragma once
#include <list>
#include "../Utils/Singleton.h"

class Session;

//-----------------------------------------------------------------
// 초당 이동 픽셀
//-----------------------------------------------------------------
#define velocityX					150
#define velocityY					100

//-----------------------------------------------------------------
// 화면 이동 영역
//-----------------------------------------------------------------
#define RANGE_MOVE_LEFT				10
#define RANGE_MOVE_RIGHT			630
#define RANGE_MOVE_TOP				50
#define RANGE_MOVE_BOTTOM			470

//-----------------------------------------------------------------
// 이동 방향
//-----------------------------------------------------------------
#define MOVE_DIR_LL					0
#define MOVE_DIR_LU					1
#define MOVE_DIR_UU					2
#define MOVE_DIR_RU					3
#define MOVE_DIR_RR					4
#define MOVE_DIR_RD					5
#define MOVE_DIR_DD					6
#define MOVE_DIR_LD					7
#define MOVE_DIR_NONE				8

//-----------------------------------------------------------------
// 이동 오류체크 범위
//-----------------------------------------------------------------
#define ERROR_RANGE				50

//---------------------------------------------------------------
// 공격범위.
//---------------------------------------------------------------
#define ATTACK1_RANGE_X		80
#define ATTACK2_RANGE_X		90
#define ATTACK3_RANGE_X		100
#define ATTACK1_RANGE_Y		10
#define ATTACK2_RANGE_Y		10
#define ATTACK3_RANGE_Y		20


#define ATTACK1_DAMAGE		10
#define ATTACK2_DAMAGE		20
#define ATTACK3_DAMAGE		30

#define MAX_HP				100


class Player
{
	friend class PacketHandler;

private:
	int playerId_;
	int sessionId_;

	int action_;
	// LL or RR
	char direction_;

	float x_;
	float y_;
	char hp_;

	bool isActive_;

	/// 메세지 쿨타임 추가 필요

public:
	void initialize(Session* session);

	int id() const;
	int sessionId() const;
	int action() const;
	int direction() const;
	float x() const;
	float y() const;
	char hp() const;
	bool isActive() const;

	void deactivate();

	void move();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};

class PlayerManager : public Singleton<PlayerManager>
{
	friend class PacketHandler;

private:
	std::list<Player*> playerList_{};
	int size_ = 0;

public:
	void createPlayer(Session* session);
	void remove(Player* player);
	void lazyDeletion();

	Player* findBySessionId(int sessionId);

	void update();
};
