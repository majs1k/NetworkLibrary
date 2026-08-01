// -------------------------------------------------------------------
// 
// 프로파일러
// 
// QueryPerformanceCounter로 인자 전달한 후 QuadPart로 시간 추출
// Profile 클래스 멤버변수들은 100ns 단위 (정밀함을 위함)
// 파일 출력은 us단위로 진행
// 
// 1. 시간을 측정해서 로직 개선 / 자료구조 수정
// 2. 호출횟수를 측정해서 자주 호출되는 함수 성능 개선 (80-20 법칙)
// 
// 스페이스바 -> 파일 저장
// C키 -> 리셋
//
// 프로그램 실행 후, 프로파일러 리셋 1회 권장
// 
// -------------------------------------------------------------------
#pragma once
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <conio.h>
#include <process.h>
//#include <timeapi.h>
#include "Singleton.h"
#include <Windows.h>

#define PROFILER

#ifdef PROFILER

#define PRO(name)				ProfileGuard pg(name)
#define PRO_BEGIN(name)			Profiler::getInstance().beginProfile(name)
#define PRO_END(name)			Profiler::getInstance().endProfile(name)

#else
#define PRO(name)
#define PRO_BEGIN(name)
#define PRO_END(name)

#endif

struct Profile
{
	std::wstring		name_;

	__int64				totalTime_;
	__int64				minTime_;
	__int64				maxTime_;
	__int64				call_;

	LARGE_INTEGER		startTime_;

public:
	Profile(const std::wstring& name)
		:name_(name), totalTime_(0), minTime_(INT_MAX), maxTime_(0), call_(0), startTime_{}
	{
	}

	std::wstring name()
	{
		return name_;
	}

	void update(__int64 time)
	{
		totalTime_ += time;
		call_++;

		if (time < minTime_)
			minTime_ = time;

		if (maxTime_ < time)
			maxTime_ = time;
	}

	void clear()
	{
		totalTime_ = 0;
		minTime_ = INT_MAX;
		maxTime_ = 0;
		call_ = 0;
	}
};

class Profiler : public Singleton<Profiler>
{
private:
	std::unordered_map<std::wstring, Profile*> profileMap_;

	LARGE_INTEGER freq_;

public:
	Profiler()
	{
		timeBeginPeriod(1);

		// 1초의 진동주기 (현재 os에서는 천만 -> 100ns 단위 측정 가능)
		QueryPerformanceFrequency(&freq_);

		//HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, this->profilerThread, this, 0, nullptr);
	}

	~Profiler()
	{
		//CloseHandle(hThread);

		for (auto& p : profileMap_)
		{
			delete p.second;
		}
	}

	void insert(Profile* profile)
	{
		profileMap_.insert({ profile->name(), profile });
	}

	void clear()
	{
		for (auto& p : profileMap_)
		{
			p.second->clear();
		}

		std::wcout << L"Profiler clear" << std::endl;
	}

	Profile* find(const std::wstring& name)
	{
		auto it = profileMap_.find(name);

		if (it != profileMap_.end())
			return (*it).second;
		else
			return nullptr;
	}

	void beginProfile(const std::wstring& name)
	{
		// Profiler 리스트에서 들고 오거나, 없다면 리스트에 추가
		Profile* profile = this->find(name);

		if (profile == nullptr)
		{
			profile = new Profile(name);

			Profiler::getInstance().insert(profile);
		}

		QueryPerformanceCounter(&(profile->startTime_));
	}

	void endProfile(const std::wstring& name)
	{
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);

		Profile* profile = this->find(name);

		if ((endTime.QuadPart - profile->startTime_.QuadPart) < 0)
		{
			DebugBreak();
		}

		// 100ns 단위 환산
		__int64 timeDiff = (__int64)((endTime.QuadPart - profile->startTime_.QuadPart) * 10'000'000 / freq_.QuadPart);

		profile->update(timeDiff);
	}

	void save()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		WCHAR fileName[MAX_PATH];
		swprintf_s(fileName, MAX_PATH, L"./Log/profiler_%04d%02d%02d_%02d%02d%02d.txt", 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		std::wofstream fout(fileName);

		if (!fout.is_open())
		{
			printf("logfile open error\n");
			printf("check file directory\n");

			__debugbreak();
		}

		fout << std::setw(15) << L"Name"
			<< std::setw(15) << L"Average(us)"
			<< std::setw(15) << L"Min(us)"
			<< std::setw(15) << L"Max(us)"
			<< std::setw(15) << L"Call"
			<< std::endl;

		fout << L"--------------------------------------------------------------------------------------" << std::endl;

		for (auto& p : profileMap_)
		{
			Profile* profile = p.second;

			if (profile->call_ <= 2)
				continue;

			fout << std::fixed << std::setprecision(3)
				<< std::setw(15) << profile->name_
				<< std::setw(15) << static_cast<double>(profile->totalTime_ - profile->minTime_ - profile->maxTime_)
				/ 10.0 / (profile->call_ - 2)
				<< std::setw(15) << static_cast<double>(profile->minTime_) / 10
				<< std::setw(15) << static_cast<double>(profile->maxTime_) / 10
				<< std::setw(15) << profile->call_
				<< std::endl;
		}

		fout.close();

		wprintf(L"profiler saved\n");
	}

	static unsigned int __stdcall profilerThread(void* param)
	{
		Profiler* profiler = reinterpret_cast<Profiler*>(param);

		while (1)
		{
			int c = _getch();

			if (c == ' ')
				profiler->save();

			else if (c == 'c')
				profiler->clear();

			else if (c == 'x')
				return 0;
		}

		return 0;
	}
};

// RAII
class ProfileGuard
{
private:
	std::wstring name_;

public:
	ProfileGuard(const std::wstring& name)
	{
		name_ = name;
		Profiler::getInstance().beginProfile(name_);
	}

	~ProfileGuard()
	{
		Profiler::getInstance().endProfile(name_);
	}
};
