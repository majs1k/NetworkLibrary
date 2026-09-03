#pragma once
#include "../../Network/LanServer.h"
#include "../../Utils/PacketQueue.h"

#include "../ResponseCode.h"
#include "../Player.h"
#include "../Icon.h"
#include "../MatchMaker.h"
#include "../GameRoom.h"

#include "../../Database/Database.h"

#include "../Repository/UserRepository.h"
#include "../Repository/PlayerRepository.h"
#include "../Repository/InventoryRepository.h"

//#include "../RPC/RpcServerProxy.h"
#include "../../RPC/RpcServerStub.h"

//#include "../Database/DbProxy.h"
#include "../../Database/DbStub.h"


class RpcServerProxy;
class DbProxy;


class GameServer : public LanServer, public RpcServerStub, public DbStub
{
private:

	PacketQueue networkQueue_;

public:

	GameServer();

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

	bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password) override;

	bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password) override;

	bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName) override;

	bool ReqPlayerConnection(__int64 sessionId, int userId) override;

	bool ReqEnterLobby(__int64 sessionId) override;


	bool ReqChat(__int64 sessionId, std::string& message) override;

	bool ReqBuyIcon(__int64 sessionId) override;

	bool ReqChangeEquipment(__int64 sessionId, int inventoryId) override;

	bool ReqStartMatch(__int64 sessionId) override;

	bool ReqCancelMatch(__int64 sessionId) override;


	bool ReqPlaceStone(__int64 sessionId, short row, short col) override;

	bool ReqLeaveRoom(__int64 sessionId) override;


	// ===================================================================================== //
	// DB
	// ===================================================================================== //

private:

	bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password) override;

	bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code) override;

	bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password) override;

	bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId) override;

	bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName) override;

	bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code) override;


	bool ReqPlayerConnectionDB(__int64 sessionId, int userId) override;

	bool ResPlayerProfileDB(__int64 sessionId, Player& player) override;

	bool ResPlayerIconsDB(__int64 sessionId, std::vector<Icon>& icons) override;


	bool ReqBuyIconDB(__int64 sessionId, int playerId, int inventoryId, int iconId, int curMoney) override;

	bool ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId) override;

	bool ReqGameResultDB(__int64 sessionId, int winnerId, int loserId) override;
};
