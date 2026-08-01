#pragma once
#include "../Network/LanServer.h"
#include "../Utils/Message.h"

// ------------------------------------------------------- //

struct TEST_BODY
{
	__int64 data_;
};

// ------------------------------------------------------- //
class Packet;
class Message;

class TestServer : public LanServer
{
	HANDLE hLogicThread_;
	bool shutdown_ = false;

	MessageQueue messageQueue_;

public:
	TestServer();

	bool onConnectionRequest(const std::wstring& ip, int port) override;
	void onAccept(__int64 sessionId) override;
	void onRelease(__int64 sessionId) override;
	void onRecv(__int64 sessionId, Packet* packet) override;
	void onRecv(__int64 sessionId, RecvPacket* packet) override;
	void onError(int errorCode, wchar_t* str) override;

	static unsigned int __stdcall logicThread(void* param);
	void packetProc();
	void update();
};
