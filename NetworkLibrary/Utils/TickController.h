// -------------------------------------------------------------------
// 
// 틱 컨트롤러
//
// ms 단위 계산
// 완벽한 고정프레임 불가 -> delteTime 변수 필요 (로직에서 활용)
// 프레임 밀리면 다음 구간에서 보정
// 
// cf. 패킷(메시지) TPS와 DB TPS 별개
//
// -------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include "Singleton.h"

#define TARGET_FPS      50

class TickController : public Singleton<TickController>
{
	DWORD frameStartTime_;
	DWORD runEndTime_;
	DWORD lastSecond_;
	int deltaTime_;

	int fps_;
	int fpsCount_;
	int targetFps_;

	int tps_;
	LONG tpsCount_;

public:
	TickController();

	float deltaTime() const;
	int fps() const;
	int tps() const;

	void setNormalFps();
	void setSlowFps();

	void update();
	void incrementTpsCount();
	void print();
};
