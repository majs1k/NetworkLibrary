#pragma once
#include "../Network/LanServer.h"
#include "../Utils/PacketQueue.h"

#include "ResponseCode.h"
#include "Player.h"
#include "Character.h"
#include "MatchMaker.h"
#include "GameRoom.h"

#include "../Database/Database.h"

#include "Repository/UserRepository.h"
#include "Repository/PlayerRepository.h"
#include "Repository/InventoryRepository.h"

//#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

#include "../Database/DbProxy.h"
#include "../Database/DbStub.h"


class RpcServerProxy;


class MyServer : public LanServer, public RpcServerStub, public DbStub
{
private:

	PacketQueue networkQueue_;

public:

	MyServer();

	void AttachProxy(RpcServerProxy* rpcProxy);
	void AttachDb(Database* db);

private:

	bool OnConnectionRequest(const std::string& ip, int port) override;
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

	// ===================================================================================== //

private:

	Database* db_;

	PacketQueue dbReqQueue_;
	PacketQueue dbResQueue_;

	HANDLE hDatabaseThread_;
	static unsigned int __stdcall DatabaseThread(void* param);

	DbProxy dbProxy_;

private:

	void ProcessDbReqQueue();
	void ProcessDbResQueue();


	// ===================================================================================== //
	// Repository
	// ===================================================================================== //

private:

	UserRepository userRepository_;
	PlayerRepository playerRepository_;
	InventoryRepository inventoryRepository_;


	// ===================================================================================== //
	// Contents
	// ===================================================================================== //

private:

	// 클라이언트에서 보내는 playerId는 신뢰할수 없음. 서버에서 sessionId를 매핑해서 알아냄
	//std::unordered_map<__int64, int> sessionToPlayer_;
	//std::unordered_map<int, Player*> playerMap_{};
	//int playerCount_ = 0;

	PlayerManager playerManager_;

	MatchMaker matchMaker_;

	GameRoomManager roomManager_;

	// ===================================================================================== //
	// RPC
	// ===================================================================================== //

private:

	bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);

	bool ReqPlayerConnection(__int64 sessionId, int userId);


	bool ReqChat(__int64 sessionId, std::string& message);

	bool ReqBuyCharacter(__int64 sessionId);

	bool ReqChangeEquipment(__int64 sessionId, int inventoryId);

	bool ReqStartGame(__int64 sessionId);

	bool ReqCancelGame(__int64 sessionId);


	// ===================================================================================== //
	// DB
	// ===================================================================================== //

private:

	bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);

	bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);

	bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);

	bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);


	bool ReqPlayerConnectionDB(__int64 sessionId, int userId);

	bool ResPlayerProfileDB(__int64 sessionId, Player& player);

	bool ResPlayerCharactersDB(__int64 sessionId, std::vector<Character>& characters);


	bool ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold);

	bool ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId);
};
