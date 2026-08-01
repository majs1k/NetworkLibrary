#include <iostream>
#include <winsock2.h>
#include "Logger.h"

Logger::Logger()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	swprintf_s(fileName_, L"./Log/log_%04d%02d%02d_%02d%02d%02d.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	fout_.open(fileName_);

	if (!fout_.is_open())
		printf("*** Logger fail ***\n");
}

Logger::~Logger()
{
	fout_.close();
}

void Logger::log(LogLevel level, const WCHAR* fmt, ...)
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

	swprintf_s(finalMessage, L"[%04d-%02d-%02d %02d:%02d:%02d.%03d] [T%u] [%s] %s\n",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		threadId, levelStr, userMessage);

	std::wcout << finalMessage;
	fout_ << finalMessage;
}

void SocketError(const WCHAR* message)
{
	wprintf(L"[SOCKET ERROR] %s : %d\n", message, WSAGetLastError());
	Sleep(INFINITE);
}
