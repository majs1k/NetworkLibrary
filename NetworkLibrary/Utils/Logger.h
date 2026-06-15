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
// wofstream 으로 파일 출력한다고 파일 인코딩은 기본 utf-8
// utf-8 원한다면 따로 설정 필요
//-------------------------------------------------------------------------
#pragma once
#include <fstream>
#include <Windows.h>
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
	std::wofstream fout_;
	WCHAR fileName_[MAX_PATH];

public:
	Logger();
	~Logger();

	void log(LogLevel level, const WCHAR* fmt, ...);
};

#define ERR(message)      SocketError(message)

void SocketError(const WCHAR* message);
