//-------------------------------------------------------------------------
// 
// 로그
// 
// 유니코드 사용해야함 주의
// Log에서 문자열 출력시 무조건 %s로 사용할것 (유니코드라도)
// 
// 예외조건 항상 꼼꼼히 체크할것 why? 멀티스레드에서는 어떤 오류가 발생할지 모름
// 서버 다운되도 데이터 오염은 never -> 로그 & 메모리덤프로 디버깅
// 
// DebugBreak()
// 디버거가 있으면 브레이크포인트 역할
// 단순히 실행파일이 실행중이면, 프로세스에서 crash 발생 (비정상 종료)
// 
// 
//-------------------------------------------------------------------------
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <fstream>
#include <windows.h>
#include "Singleton.h"

enum class LogLevel
{
    DEFAULT,
    INFO,
    ERR,
};

#define LOG(fmt, ...)		        Logger::getInstance().log(LogLevel::DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)          Logger::getInstance().log(LogLevel::INFO, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)         Logger::getInstance().log(LogLevel::ERR, fmt, ##__VA_ARGS__)

class Logger : public Singleton<Logger>
{
private:
	std::wofstream fout_;		// w 주의
	WCHAR fileName_[MAX_PATH];

public:
	Logger()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		swprintf_s(fileName_, L"./Log/log_%04d%02d%02d_%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		fout_.open(fileName_);

		if (!fout_.is_open())
			printf("*** Logger fail ***\n");
		else
			printf("### Logger OK ###\n");
	}

	~Logger()
	{
		fout_.close();
	}

    void log(LogLevel level, const WCHAR* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        WCHAR userMessage[1024];
        vswprintf_s(userMessage, fmt, args);

        va_end(args);

        SYSTEMTIME st;
        GetLocalTime(&st);

        DWORD threadId = GetCurrentThreadId();

        const WCHAR* levelStr;

        switch (level)
        {
        case LogLevel::INFO:
            levelStr = L"INFO";
            break;

        case LogLevel::ERR:
            levelStr = L"ERROR";
            break;

        default:
            levelStr = L"DEFAULT";
            break;
        }

        WCHAR finalMessage[1400];

        swprintf_s(
            finalMessage,
            L"[%04d-%02d-%02d %02d:%02d:%02d.%03d] [T%u] [%s] %s\n",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds,
            threadId,
            levelStr,
            userMessage);

        std::wcout << finalMessage;
        fout_ << finalMessage;
    }
};

#define ERR(message)      SocketError(message)

inline void SocketError(const WCHAR* message)
{
	wprintf(L"[SOCKET ERROR] %s : %d\n", message, WSAGetLastError());
	Sleep(INFINITE);
}
