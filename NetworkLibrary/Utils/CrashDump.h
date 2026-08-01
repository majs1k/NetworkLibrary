//-------------------------------------------------------------------------
//
// 크래시 덤프
// 
// initialize() 호출 필요
// 덤프 + exe + pdb 세트로 보관
// 덤프 파일사이즈 == 프로세스의 메모리 사용량
// 
// 네이티브 전용 디버그 사용
// 
//-------------------------------------------------------------------------
#pragma comment(lib, "Dbghelp.lib")
#include <stdio.h>
#include <Windows.h>
#include <minidumpapiset.h>
#include <crtdbg.h>

class CrashDump
{
public:
	static void initialize()
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilter);

		_set_invalid_parameter_handler(InvalidParameterHandler);
		_set_purecall_handler(PureCallHandler);

		_CrtSetReportMode(_CRT_WARN, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_CrtSetReportMode(_CRT_ERROR, 0);

		_CrtSetReportHook(CrtReportHook);
	}

private:
	static LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
	{
		if (InterlockedExchange(&s_Dumping, 1) != 0)
		{
			Sleep(INFINITE);
		}

		CreateDump(exceptionInfo);

		return EXCEPTION_EXECUTE_HANDLER;
	}

	static void CreateDump(EXCEPTION_POINTERS* exceptionInfo)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		wprintf(L"\n\ncrash error : %04d.%02d.%02d/%02d.%02d.%02d\n",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);

		WCHAR fileName[MAX_PATH];

		wsprintfW(fileName, L"Dump_%04d%02d%02d_%02d%02d%02d.dmp",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);

		HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			return;

		MINIDUMP_EXCEPTION_INFORMATION mei;
		mei.ThreadId = GetCurrentThreadId();
		mei.ExceptionPointers = exceptionInfo;
		mei.ClientPointers = FALSE;

		BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hFile, MiniDumpWithFullMemory, exceptionInfo ? &mei : nullptr, nullptr, nullptr);

#ifdef _DEBUG
		if (!success)
		{
			DWORD err = GetLastError();

			WCHAR msg[256];
			wsprintfW(msg, L"MiniDumpWriteDump Failed : %u\n", err);

			OutputDebugStringW(msg);
		}
#endif

		CloseHandle(hFile);

		wprintf(L"dump save finish\n");
	}

	static void ForceCrash()
	{
		RaiseException(EXCEPTION_NONCONTINUABLE_EXCEPTION, 0, 0, nullptr);
	}

	static void InvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
	{
		ForceCrash();
	}

	static void PureCallHandler()
	{
		ForceCrash();
	}

	static int CrtReportHook(int, char*, int*)
	{
		ForceCrash();
		return TRUE;
	}

private:
	static LONG s_Dumping;
};

LONG CrashDump::s_Dumping = 0;
