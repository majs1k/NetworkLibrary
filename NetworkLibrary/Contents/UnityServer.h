#pragma once
#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"
#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"


class Packet;
class Message;


class UnityServer : public LanServer, public RpcServerStub
{
	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue packetQueue_;

	RpcServerProxy proxy_;

public:
	UnityServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	// ----------------------------------------------------- //

	bool LoginRequest(__int64 sessionId, int id);

	bool LoginResponse(__int64 sessionId, int id);
};
