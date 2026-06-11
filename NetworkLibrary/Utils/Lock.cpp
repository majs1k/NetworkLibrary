#include "Lock.h"

Lock::Lock()
{
	InitializeCriticalSection(&cs_);
}

Lock::~Lock()
{
	DeleteCriticalSection(&cs_);
}

void Lock::lock()
{
	EnterCriticalSection(&cs_);
}

void Lock::unlock()
{
	LeaveCriticalSection(&cs_);
}
