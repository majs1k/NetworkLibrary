#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Lock
{
private:
	CRITICAL_SECTION cs_;

public:
	Lock()
	{
		InitializeCriticalSection(&cs_);
	}

	~Lock()
	{
		DeleteCriticalSection(&cs_);
	}

	void lock()
	{
		EnterCriticalSection(&cs_);
	}

	void unlock()
	{
		LeaveCriticalSection(&cs_);
	}
};
