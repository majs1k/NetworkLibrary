#pragma once
#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"

// ------------------------------------------------------- //
class Packet;
class Message;

class TestServer : public LanServer
{
	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue packetQueue_;

public:
	TestServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void PacketProc();
	void Update();
};
