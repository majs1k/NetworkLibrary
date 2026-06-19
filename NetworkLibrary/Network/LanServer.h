#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <vector>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "../Utils/Lock.h"
#include <Windows.h>



// ------------------------------------------------------- //

#define PACKET_CODE				0x89

struct FIGHTER_HEADER
{
	unsigned char code;
	unsigned char size;
	//unsigned char type;
};

// ------------------------------------------------------- //

class Session;
class Packet;
class Buffer;

class LanServer
{
private:
	SOCKET listenSocket_;

	std::wstring serverIp_;
	int serverPort_;

	HANDLE hIOCP_;
	std::vector<HANDLE> hWorkerThread_;
	HANDLE hAcceptThread_;
	HANDLE hMonitorThread_;
	HANDLE hExitEvent;
	int concurrentThreadCount_;
	int workerThreadCount_;

	// rehash 일어나면(갑자기 삽입 했을때 등..) 이터레이터 무효화 주의
	std::unordered_map<__int64, Session*> sessionMap_;
	LONG sessionCount_ = 0;
	int sessionMax_;
	Lock sessionMapLock_;
	__int64 idSeed_ = 0;

	LONG acceptTps_;
	LONG recvMessageTps_;
	LONG sendMessageTps_;

	LONG acceptCount_;
	LONG recvMessageCount_;
	LONG sendMessageCount_;

public:
	LanServer();
	~LanServer();

	bool start(std::wstring ip, int port, int sessionMax, int concurrentCount, int workerCount);
	void stop();

	virtual bool defaultStart();
	virtual void defaultStop();
	int sessionCount() const;
	int sessionMax() const;

	/// false가 반환되면 뭘 해야함???
	bool disconnect(__int64 sessionId);
	bool sendPacket(__int64 sessionId, Packet* packet);

	// (외부/해외/공격)IP 차단 기능 + 패치 후 점검 white ip만 가능케
	virtual bool onConnectionRequest(const std::wstring& ip, int port) = 0;
	// 인자 미정
	// Session 포인터, socket -> 컨텐츠로 절대 전달 x
	virtual void onAccept(__int64 sessionId) = 0;
	// release후, 즉 세션 삭제 후 호출됨 주의
	virtual void onRelease(__int64 sessionId) = 0;

	virtual void onRecv(__int64 sessionId, Packet& packet) = 0;

	// 컨텐츠에게 에러코드를 알려주지만 서버 끌 상황은 아닐때
	virtual void onError(int errorCode, wchar_t* str) = 0;

	int acceptTps();
	int recvMessageTps();
	int sendMessageTps();
	void printTps();

	static unsigned int __stdcall acceptThread(void* param);
	static unsigned int __stdcall workerThread(void* param);
	static unsigned int __stdcall mornitorThread(void* param);

private:
	void postRecv(Session* session);
	void completeRecv(Session* session, int numOfBytes);
	void postSend(Session* session);
	void completeSend(Session* session, int numOfBytes);
	void decrementIoCount(Session* session);
	void decrementPacketCount(Packet* packet);
};
