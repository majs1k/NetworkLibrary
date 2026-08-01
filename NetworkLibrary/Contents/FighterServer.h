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

	bool onConnectionRequest(const std::wstring& ip, int port) override;
	void onAccept(__int64 sessionId) override;
	void onRelease(__int64 sessionId) override;
	void onRecv(__int64 sessionId, Packet* packet) override;
	void onRecv(__int64 sessionId, RecvPacket* packet) override;
	void onError(int errorCode, wchar_t* str) override;

	static unsigned int __stdcall logicThread(void* param);
	void update();

	// ----------------------------------------------------- //

	void createPlayer(__int64 sessionId);
	void removePlayer(__int64 sessionId);

	// ----------------------------------------------------- //

	bool cs_start_move(__int64 sessionId, char direction, short x, short y);
	bool cs_stop_move(__int64 sessionId, char action, short x, short y);
	bool cs_attack1(__int64 sessionId, char direction, short x, short y);
};
