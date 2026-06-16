#pragma once
#include <process.h>
#include <windows.h>
#include "Player.h"
#include "../Utils/TickController.h"

/// 전역으로 링버퍼 하나 두고 메세지 받아서 처리
class LogicThread
{
private:
	HANDLE hLogicThread_;
	bool shutdown_ = false;

public:
	void start()
	{
		hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, logicThread, this, 0, nullptr);
	}

	void stop()
	{
		shutdown_ = true;
	}

	static unsigned int __stdcall logicThread(void* param)
	{
		LogicThread* t = (LogicThread*)param;

		while (!t->shutdown_)
		{
			PlayerManager::getInstance().update();

			TickController::getInstance().update();
		}

		return 0;
	}
};
