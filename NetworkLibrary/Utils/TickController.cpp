#pragma comment(lib, "winmm.lib")
#include <iostream>
#include "TickController.h"

TickController::TickController()
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

	tps_ = 0;
	tpsCount_ = 0;
}

float TickController::deltaTime() const
{
	return deltaTime_;
}

int TickController::fps() const
{
	return fps_;
}

int TickController::tps() const
{
	return tps_;
}

void TickController::setNormalFps()
{
	targetFps_ = TARGET_FPS;
}

void TickController::setSlowFps()
{
	targetFps_ = TARGET_FPS / 2;
}

void TickController::update()
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

		tps_ = InterlockedExchange(&tpsCount_, 0);
	}
}

void TickController::incrementTpsCount()
{
	InterlockedIncrement(&tpsCount_);
}

void TickController::print()
{
	printf("[Profile] FPS  : %d\n", fps_);
	printf("[Profile] TPS  : %d\n", tps_);
	printf("---------------------------------\n");
}
