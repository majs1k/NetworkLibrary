#pragma once
#include "../Network/LanServer.h"
#include "../RPC/RPCProxy.h"
#include "../RPC/RPCStub.h"

extern RPCProxy proxy;

class FighterServer : public LanServer
{
private:
	RPCStub stub;

public:
	FighterServer();

	bool onConnectionRequest(const std::wstring& ip, int port) override;

	void onAccept(__int64 sessionId) override;

	void onRelease(__int64 sessionId) override;

	void onRecv(__int64 sessionId, Packet& packet) override;

	void onError(int errorCode, wchar_t* str) override;
};
