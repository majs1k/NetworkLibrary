// -----------------------------------------------------------------
// 
// 메모리 누수 탐지
// 
// new delete 연산자 오버로딩 활용
// 연산자 오버로딩 시, stl 사용 불가 (오버로딩한 new연산자가 재귀호출됨)
// 
// -----------------------------------------------------------------
#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <Windows.h>
#include "Singleton.h"

struct MemoryAlloc
{
	void* ptr;
	size_t size;
	char filename[128];
	int  line;

	MemoryAlloc(void* ptr, size_t size, const char* fileName, int line)
		:ptr(ptr), size(size), line(line)
	{
		strcpy_s(filename, 128, fileName);
	};
};

class MemoryLeak : public Singleton<MemoryLeak>
{
private:
	std::list<MemoryAlloc*> list_;

public:
	void Insert(void* ptr, size_t size, const char* fileName, int line)
	{
		MemoryAlloc* p = new MemoryAlloc(ptr, size, fileName, line);
		list_.push_back(p);
	}

	void Remove(void* ptr)
	{
		std::list<MemoryAlloc*>::iterator it = list_.begin();
		for (; it != list_.end();)
		{
			if ((*it)->ptr == ptr)
			{
				delete (*it);
				it = list_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	~MemoryLeak()
	{
		SYSTEMTIME st;

		GetLocalTime(&st);
		char logFileName[256];
		sprintf_s(logFileName, "Alloc_%04d%02d%02d_%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		FILE* fp;
		fopen_s(&fp, logFileName, "wt");
		if (fp == nullptr)
			return;

		for (auto ai : list_)
		{
			fprintf(fp, "### LEAK [%p] [%d] %s : %d\n", ai->ptr, ai->size, ai->filename, ai->line);
		}

		fclose(fp);
	}
};

inline void* operator new (size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);

	MemoryLeak::Instance().Insert(ptr, size, File, Line);

	return ptr;
}

inline void* operator new[](size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);

	MemoryLeak::Instance().Insert(ptr, size, File, Line);

	return ptr;
}

inline void operator delete (void* ptr, const char* File, int Line)
{
}

inline void operator delete[](void* ptr, const char* File, int Line)
{
}

// 실제로 사용할 delete
inline void operator delete (void* ptr)
{
	MemoryLeak::Instance().Remove(ptr);

	free(ptr);
}

inline void operator delete[](void* ptr)
{
	MemoryLeak::Instance().Remove(ptr);

	free(ptr);
}

//#define MEMORY_LEAK
#ifdef MEMORY_LEAK
#define new new(__FILE__, __LINE__)
#endif
