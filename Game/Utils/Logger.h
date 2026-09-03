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
// wofstream 파일 출력해도 파일 인코딩은 utf-8
// 
//-------------------------------------------------------------------------
#pragma once
#include <fstream>
#include "Singleton.h"
#include <Windows.h>

enum class LogLevel
{
	DEFAULT,
	INFO,
	ERR,
};

#define LOG(fmt, ...)		        Logger::Instance().Log(LogLevel::DEFAULT, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)          Logger::Instance().Log(LogLevel::INFO, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)         Logger::Instance().Log(LogLevel::ERR, fmt, ##__VA_ARGS__)

class Logger : public Singleton<Logger>
{
private:
	std::wofstream fout_;
	WCHAR fileName_[MAX_PATH];

public:
	Logger();
	~Logger();

	void Log(LogLevel level, const WCHAR* fmt, ...);
};
