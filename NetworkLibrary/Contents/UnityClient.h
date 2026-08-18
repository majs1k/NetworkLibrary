#pragma once
#include "../Network/LanClient.h"
#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

class UnityClient : public LanClient, public RpcServerStub
{
public:
	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;


	void SendTest();

	// ----------------------------------------------------- //

	bool LoginRequest(__int64 sessionId, int id);

	bool LoginResponse(__int64 sessionId, int id);
};
