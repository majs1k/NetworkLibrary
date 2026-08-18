#pragma once
#include <list>
#include "../Network/LanClient.h"

#include "../RPC/RpcClientProxy.h"
#include "../RPC/RpcClientStub.h"


class UnityClient : public LanClient, public RpcClientStub
{
private:
	RpcClientProxy rpc_;

public:
	UnityClient();

	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;


	void SendTest();

	// ----------------------------------------------------- //

	bool LoginRequest(int id) override;

	bool LoginResponse(int id) override;

	bool ChatRequest(std::string& chat) override;

	bool ChatResponse(std::string& chat) override;

	bool ItemRequest(std::list<int>& lst) override;

	bool ItemResponse(std::list<int>& lst) override;
};
