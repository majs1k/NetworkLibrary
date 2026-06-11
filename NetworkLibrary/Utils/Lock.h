#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Lock
{
private:
	CRITICAL_SECTION cs_;

public:
	Lock();

	~Lock();

	void lock();

	void unlock();
};
