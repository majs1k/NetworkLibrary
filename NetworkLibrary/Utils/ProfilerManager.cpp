#include "ProfilerManager.h"
#include <process.h>
#include "Profiler.h"

ProfilerManager::ProfilerManager()
{
	 //hThread_ = (HANDLE)_beginthreadex(nullptr, 0, this->ProfilerThread, this, 0, nullptr);
}

ProfilerManager::~ProfilerManager()
{
	CloseHandle(hThread_);
}

void ProfilerManager::RegisterProfiler(Profiler* profiler)
{
	lock_.lock();

	profilers_.push_back(profiler);

	lock_.unlock();
}

void ProfilerManager::Save()
{
	for (auto& p : profilers_)
		p->Save();

	wprintf(L"profiler saved\n");
}

void ProfilerManager::Clear()
{
	for (auto& p : profilers_)
		p->Clear();

	wprintf(L"profiler clear\n");
}

unsigned int __stdcall ProfilerManager::ProfilerThread(void* param)
{
	ProfilerManager* pm = reinterpret_cast<ProfilerManager*>(param);

	while (1)
	{
		int c = _getch();

		if (c == ' ')
		{
			pm->Save();
		}

		else if (c == 'c')
		{
			pm->Clear();
		}

		else if (c == 'x')
			return 0;
	}

	return 0;
}
