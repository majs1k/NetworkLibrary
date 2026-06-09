#pragma once
#include <windows.h>
#include "Singleton.h"

//#define MEMORY_OVERFLOW

#ifdef	MEMORY_OVERFLOW
#define new		MemoryOverflow::getInstance().new
#endif

class MemoryOverflow : Singleton <MemoryOverflow>
{
public:
	static char* alloc(int size)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		if (size <= 0) 
			return nullptr;

		DWORD pageSize = si.dwPageSize;

		size_t dataPages = (size + 4 - 1 + pageSize) / pageSize;

		char* base = (char*)VirtualAlloc(nullptr, (dataPages + 1) * pageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (!base) 
			return nullptr;

		char* noaccessPtr = base + dataPages * pageSize;

		VirtualAlloc(noaccessPtr, pageSize, MEM_COMMIT, PAGE_NOACCESS);

		char* retPtr = noaccessPtr - size;

		*((int*)retPtr - 1) = (unsigned int)base;

		return retPtr;
	}

	static void free(void* ptr)
	{
		int* p = (int*)ptr - 1;
		char* addr = (char*)*p;
		VirtualFree(addr, 0, MEM_RELEASE);
	}

	// 반드시 size_t 여야 함
	static void* operator new(size_t size)
	{
		void* ptr = alloc(size);
		return ptr;
	}

	static void operator delete(void* ptr)
	{
		free(ptr);
	}
};
