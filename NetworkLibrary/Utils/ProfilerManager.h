#pragma once
#include <vector>
#include <mutex>

#include "Singleton.h"
#include <Windows.h>

class Profiler;

class ProfilerManager : public Singleton<ProfilerManager>
{
public:
	std::vector<Profiler*> profilers_;

	std::mutex lock_;
	HANDLE hThread_;

public:
	ProfilerManager();
	~ProfilerManager();

	void RegisterProfiler(Profiler* profiler);

	void Save();

	void Clear();

	static unsigned int __stdcall ProfilerThread(void* param);
};
