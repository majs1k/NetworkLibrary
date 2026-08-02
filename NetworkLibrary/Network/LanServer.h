#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
//#include <Windows.h>

// ------------------------------------------------------- //

struct TEST_HEADER
{
	short size_;
};

// ------------------------------------------------------- //

#define PACKET_CODE				0x89

struct FIGHTER_HEADER
{
	unsigned char code;
	char size;
	//char type;
};

// ------------------------------------------------------- //

class Session;
class Packet;
class RecvPacket;
class PacketBuffer;

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
	std::recursive_mutex sessionMapLock_;
	__int64 idSeed_ = 0;

	LONG acceptCount_;
	LONG recvMessageCount_;
	LONG sendMessageCount_;

public:
	LanServer();
	~LanServer();

	bool Start(std::wstring ip, int port, int sessionMax, int concurrentCount, int workerCount);
	void Stop();

	int SessionCount() const;
	int SessionMax() const;

	/// false가 반환되면 뭘 해야함???
	bool Disconnect(__int64 sessionId);
	bool SendPacket(__int64 sessionId, Packet* packet);
	bool SendPacketZeroCopy(__int64 sessionId, Packet* packet);

	// (외부/해외/공격)IP 차단 기능 + 패치 후 점검 white ip만 가능케
	virtual bool OnConnectionRequest(const std::wstring& ip, int port) = 0;
	// 인자 미정
	// Session 포인터, socket -> 컨텐츠로 절대 전달 x
	virtual void OnAccept(__int64 sessionId) = 0;
	// release 후, 즉 세션 삭제 후 호출 (다른 사용자들에게 세션 종료를 알림)
	virtual void OnRelease(__int64 sessionId) = 0;

	virtual void OnRecv(__int64 sessionId, Packet* packet) = 0;
	virtual void OnRecv(__int64 sessionId, RecvPacket* packet) = 0;

	// 컨텐츠에게 에러코드를 알려주지만 서버 끌 상황은 아닐때
	virtual void OnError(int errorCode, wchar_t* str) = 0;

private:
	static unsigned int __stdcall AcceptThread(void* param);
	static unsigned int __stdcall WorkerThread(void* param);
	static unsigned int __stdcall MornitorThread(void* param);

	void RecvPost(Session* session);
	void CompleteRecv(Session* session, int numOfBytes);

	void SendPost(Session* session);
	void CompleteSend(Session* session, int numOfBytes);

	void SendPostZeroCopy(Session* session);
	void CompleteSendZeroCopy(Session* session, int numOfBytes);

	void IncrementIoCount(Session* session);
	void DecrementIoCount(Session* session);

	void ReleaseSession(Session* session);
};
