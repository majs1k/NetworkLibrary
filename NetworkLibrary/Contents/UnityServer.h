#pragma once
#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"
#include "../RPC/ServerProxy.h"
#include "../RPC/IRpcStub.h"


class Packet;
class Message;


class UnityServer : public LanServer, public IRpcStub
{
	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue packetQueue_;

	ServerProxy proxy_;

public:
	UnityServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	// ----------------------------------------------------- //

	bool cs_create_login(__int64 sessionId, int num);

	bool sc_create_login(__int64 sessionId, int id, int num);
};
