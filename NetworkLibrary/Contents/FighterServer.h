#pragma once
#include <unordered_map>
#include "../Network/LanServer.h"
#include "../RPC/ServerProxy.h"
#include "../RPC/ServerStub.h"

class Player;

class FighterServer : public LanServer, public ServerStub
{
private:
	ServerProxy proxy_;

	HANDLE hLogicThread_;
	bool shutdown_ = false;

	std::unordered_map<__int64, Player*> playerMap_{};
	int playerCount_ = 0;

public:
	FighterServer();

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	// ----------------------------------------------------- //

	void CreatePlayer(__int64 sessionId);
	void RemovePlayer(__int64 sessionId);

	// ----------------------------------------------------- //

	bool cs_start_move(__int64 sessionId, char direction, short x, short y);
	bool cs_stop_move(__int64 sessionId, char action, short x, short y);
	bool cs_attack1(__int64 sessionId, char direction, short x, short y);
};
