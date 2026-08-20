#pragma once
#include <list>
#include "../Network/LanClient.h"

#include "ResponseCode.h"

#include "../RPC/RpcClientProxy.h"
#include "../RPC/RpcClientStub.h"



class UnityClient : public LanClient, public RpcClientHandler
{
private:
	RpcClientProxy rpc_;
	RpcClientStub stub_;

public:
	UnityClient();

	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;

	// ----------------------------------------------------- //

	void TestRegister(std::string& loginId, std::string& password);
	void TestLogin(std::string& loginId, std::string& password);

	// ----------------------------------------------------- //

	bool ReqRegister(std::string& loginId, std::string& password);
	bool ResRegister(RESPONSE_CODE code);
	bool ReqLogin(std::string& loginId, std::string& password);
	bool ResLogin(RESPONSE_CODE code);

};
