#pragma once
#include "../Network/LanClient.h"
#include "../RPC/ServerProxy.h"
#include "../RPC/IRpcStub.h"

class UnityClient : public LanClient, public IRpcStub
{
public:
	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;


	void SendTest();

	// ----------------------------------------------------- //

	bool cs_create_login(__int64 sessionId, int num);

	bool sc_create_login(__int64 sessionId, int id, int num);
};
