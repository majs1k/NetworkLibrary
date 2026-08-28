#pragma once
#include <list>

#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"

#include "ResponseCode.h"
#include "../Database/Database.h"


#include "Player.h"
#include "Character.h"

#include "Repository/UserRepository.h"
#include "Repository/PlayerRepository.h"
#include "Repository/InventoryRepository.h"



//#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

//#include "../Database/DatabaseServerProxy.h"
#include "../Database/DatabaseServerStub.h"

class RpcServerProxy;
class DatabaseServerProxy;


class MyServer : public LanServer, public RpcServerHandler, public DatabaseServerHandler
{
private:

	PacketQueue networkPacketQueue_;

	RpcServerProxy* rpcProxy_;
	RpcServerStub* rpcStub_;

public:

	MyServer();
	~MyServer();

private:

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;


	void ProcessNetworkQueue();


private:

	HANDLE hLogicThread_;
	bool shutdown_ = false;

private:

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	// ----------------------------------------------------- //

private:

	Database db_;

	PacketQueue dbReqQueue_;
	PacketQueue dbResQueue_;

	HANDLE hDatabaseThread_;
	static unsigned int __stdcall DatabaseThread(void* param);

	DatabaseServerProxy* dbProxy_;
	DatabaseServerStub* dbStub_;


	UserRepository userRepository_;
	PlayerRepository playerRepository_;
	InventoryRepository inventoryRepository_;

private:

	void ProcessDbReqQueue();
	void ProcessDbResQueue();

	// ----------------------------------------------------- //

private:

	bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);

	bool ReqPlayerEnterLobby(__int64 sessionId, int userId);


	bool ReqChat(__int64 sessionId, std::string& message);

	// ----------------------------------------------------- //


private:

	bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);

	bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);

	bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);

	bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
	


	bool ReqPlayerEnterLobbyDB(__int64 sessionId, int userId);

	bool ResPlayerProfileDB(__int64 sessionId, Player& player);

	bool ResPlayerCharactersDB(__int64 sessionId, std::list<Character>& characters);


	// ----------------------------------------------------- //

private:

	// 클라이언트에서 보내는 playerId는 신뢰할수 없음. 서버에서 sessionId를 매핑해서 알아냄
	std::unordered_map<__int64, int> sessionToPlayer_;
	std::unordered_map<int, Player*> playerMap_{};
	int playerCount_ = 0;
};
