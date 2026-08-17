#pragma once
#include "../Network/LanClient.h"

class TestClient : public LanClient
{
	__int64 seed_;

public:
	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;

	void SendTestNum();
};
