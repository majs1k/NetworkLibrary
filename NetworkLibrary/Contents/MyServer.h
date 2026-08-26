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



#include "../RPC/RpcServerProxy.h"
#include "../RPC/RpcServerStub.h"

#include "../Database/DatabaseServerProxy.h"
#include "../Database/DatabaseServerStub.h"


#include <string>
#include <queue>
#include <unordered_map>



class MyServer : public LanServer, public RpcServerHandler, public DatabaseServerHandler
{
private:

	HANDLE hLogicThread_;
	bool shutdown_ = false;

	PacketQueue networkPacketQueue_;

	RpcServerProxy rpcProxy_;
	RpcServerStub rpcStub_;

public:

	MyServer();

private:

	bool OnConnectionRequest(const std::wstring& ip, int port) override;
	void OnAccept(__int64 sessionId) override;
	void OnRelease(__int64 sessionId) override;
	void OnRecv(__int64 sessionId, Packet* packet) override;

	static unsigned int __stdcall LogicThread(void* param);
	void Update();

	void ProcessNetworkQueue();

	// ----------------------------------------------------- //

private:

	Database db_;

	PacketQueue dbReqQueue_;
	PacketQueue dbResQueue_;

	HANDLE hDatabaseThread_;
	static unsigned int __stdcall DatabaseThread(void* param);

	DatabaseServerProxy dbProxy_;
	DatabaseServerStub dbStub_;


	UserRepository userRepository_;
	PlayerRepository playerRepository_;
	InventoryRepository inventoryRepository_;


private:

	void ProcessDbReqQueue();
	void ProcessDbResQueue();

	// ----------------------------------------------------- //

public:

	bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);

	bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);

	bool ReqPlayerProfile(__int64 sessionId, int userId);

	bool ReqCharacterList(__int64 sessionId, int playerId);

	bool ReqPlayerList(__int64 sessionId);

	bool ReqChat(__int64 sessionId, std::string& message);

	// ----------------------------------------------------- //


public:

	bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);

	bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);

	bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);

	bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);

	bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);

	bool ReqPlayerProfileDB(__int64 sessionId, int userId);

	bool ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player);

	bool ReqCharacterListDB(__int64 sessionId, int playerId);

	bool ResCharacterListDB(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList);

	// ----------------------------------------------------- //

private:

	std::unordered_map<__int64, int> sessionToPlayer_;
	std::unordered_map<int, Player*> playerMap_{};
	int playerCount_ = 0;

};
