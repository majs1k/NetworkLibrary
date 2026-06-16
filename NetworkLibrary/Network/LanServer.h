#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <vector>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "../Utils/RingBuffer.h"
#include "../Utils/Lock.h"

// ------------------------------------------------------- //

struct HEADER
{
	unsigned short size;
};

// ------------------------------------------------------- //

#define PACKET_CODE				0x89

struct FIGHTER_HEADER
{
	unsigned char code;
	unsigned char size;
	//unsigned char type;
};

// ------------------------------------------------------- //

#define SEND_SIZE				10000
#define RECV_SIZE				2000

enum class IOType
{
	SEND = 0,
	RECV = 1,
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped;
	IOType type;
};

/// TODO: 세션 삭제시 소켓 close
struct Session
{
	SOCKET socket_;
	// 고유값. sessionList의 인덱스로 활용 x
	// 네트워크 연결에 대한 로직/로그에서 사용
	// 소켓핸들처럼 컨텐츠에서 사용 (컨텐츠 코드가 간단해짐)
	__int64 sessionId_;

	std::wstring ip_;
	int port_;

	RingBuffer sendQueue_{ SEND_SIZE };
	RingBuffer recvQueue_{ RECV_SIZE };

	OverlappedEx sendOverlapped_;
	OverlappedEx recvOverlapped_;

	LONG ioCount_;
	LONG sendPending_;

	Lock sessionLock_;

	void initialize(SOCKET socket, std::wstring ip, int port, __int64 id)
	{
		socket_ = socket;
		sessionId_ = id;

		ip_ = ip;
		port_ = port;

		sendQueue_.clear();
		recvQueue_.clear();

		sendOverlapped_.type = IOType::SEND;
		recvOverlapped_.type = IOType::RECV;

		ioCount_ = 0;
		sendPending_ = false;
	}
};

// ------------------------------------------------------- //

class Packet;

class LanServer
{
protected:
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
	int sessionCount() const;

	/// false가 반환되면 뭘 해야함???
	bool disconnect(__int64 sessionId);
	bool sendPacket(__int64 sessionId, Packet& packet);

	// (외부/해외/공격)IP 차단 기능 + 패치 후 점검 white ip만 가능케
	virtual bool onConnectionRequest(const std::wstring& ip, int port) = 0;

	// 인자 미정
	// Session 포인터, socket -> 컨텐츠로 절대 전달 x
	virtual void onAccept(__int64 sessionId) = 0;

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
};
