#pragma once
#include <list>

#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"

#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

class UnityServer : public LanServer, public RpcServerStub
{
	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue packetQueue_;

	RpcServerProxy rpc_;

public:
	UnityServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	// ----------------------------------------------------- //

	bool LoginRequest(__int64 sessionId, int id) override;

	bool LoginResponse(__int64 sessionId, int id) override;

	bool ChatRequest(__int64 sessionId, std::string& chat) override;

	bool ChatResponse(__int64 sessionId, std::string& chat) override;

	bool ItemRequest(__int64 sessionId, std::list<int>& lst) override;

	bool ItemResponse(__int64 sessionId, std::list<int>& lst) override;
};
