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
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include "Singleton.h"
#include <Windows.h>

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

public:
	TickController()
	{
		// 시간 해상도 높여야 정확한 Sleep 가능
		timeBeginPeriod(1);

		int time = timeGetTime();
		frameStartTime_ = time;
		runEndTime_ = time;
		lastSecond_ = time;
		deltaTime_ = 0;

		fpsCount_ = 0;
		targetFps_ = TARGET_FPS;
		fps_ = 0;
	}

	float DeltaTime() const
	{
		return deltaTime_;
	}

	int Fps() const
	{
		return fps_;
	}

	void SetNormalFps()
	{
		targetFps_ = TARGET_FPS;
	}

	void SetSlowFps()
	{
		targetFps_ = TARGET_FPS / 2;
	}

	void Update()
	{
		DWORD lastRunEndTime = runEndTime_;
		runEndTime_ = timeGetTime();

		deltaTime_ = runEndTime_ - lastRunEndTime;

		int frameRunTime = runEndTime_ - frameStartTime_;

		// 시간차는 int형으로 선언
		int frameSpareTime = CLOCKS_PER_SEC / targetFps_ - frameRunTime;

		if (frameSpareTime > 0)
		{
			Sleep(frameSpareTime);
		}

		frameStartTime_ += CLOCKS_PER_SEC / targetFps_;

		fpsCount_++;

		if (CLOCKS_PER_SEC <= runEndTime_ - lastSecond_)
		{
			lastSecond_ += CLOCKS_PER_SEC;

			fps_ = fpsCount_;
			fpsCount_ = 0;
		}
	}

	void Print()
	{
		printf("[Profile] FPS  : %d\n", fps_);
		printf("---------------------------------\n");
	}
};
