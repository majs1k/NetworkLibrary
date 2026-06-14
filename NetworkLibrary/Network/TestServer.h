#pragma once
#include "LanServer.h"

// ------------------------------------------------------- //

struct MESSAGE
{
	__int64 data_;
};

// ------------------------------------------------------- //

class TestServer : public LanServer
{
	bool onConnectionRequest(const std::wstring& ip, int port) override;

	void onAccept(__int64 sessionId) override;

	void onRelease(__int64 sessionId) override;

	void onRecv(__int64 sessionId, Packet& packet) override;

	void onError(int errorCode, wchar_t* str) override;
};
