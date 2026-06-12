#pragma once
#include <process.h>
#include <windows.h>
#include "../Utils/TickController.h"

/// 전역으로 링버퍼 하나 두고 메세지 받아서 처리하자.

class GameThread
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
		GameThread* t = (GameThread*)param;

		while (!t->shutdown_)
		{


			TickController::getInstance().update();
		}

		return 0;
	}
};
