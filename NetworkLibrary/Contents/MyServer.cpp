#include "MyServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <process.h>
#include <Windows.h>


MyServer::MyServer()
{
	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	hDatabaseThread_ = (HANDLE)_beginthreadex(nullptr, 0, DatabaseThread, this, 0, nullptr);

	rpcProxy_.server_ = this;
	rpcStub_.handler_ = this;

	dbProxy_.dbQueue_ = &dbReqQueue_;
	dbProxy_.logicQueue_ = &dbResQueue_;

	dbStub_.handler_ = this;


	db_.Connect();

	userRepository_.Initialize(&db_);
	playerRepository_.Initialize(&db_);
	inventoryRepository_.Initialize(&db_);
}

bool MyServer::OnConnectionRequest(const std::wstring& ip, int port)
{
	if (SessionCount() >= SessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void MyServer::OnAccept(__int64 sessionId)
{

}

void MyServer::OnRelease(__int64 sessionId)
{
	auto it1 = sessionToPlayer_.find(sessionId);

	// 현재 플레이어 리스트에 등록되어 있다면 삭제
	if (it1 != sessionToPlayer_.end())
	{
		playerMap_.erase(sessionToPlayer_[sessionId]);

		sessionToPlayer_.erase(sessionId);
	}
}

void MyServer::OnRecv(__int64 sessionId, Packet* packet)
{
	//if (!stub_.PacketProc(sessionId, packet))
	//{
	//	//disconnect(sessionId);

	//	return;
	//}

	//delete packet;

	/// 로직 스레드에서 패킷 처리
	packet->SetId(sessionId);

	networkPacketQueue_.Push(packet);
}

unsigned int __stdcall MyServer::LogicThread(void* param)
{
	MyServer* server = static_cast<MyServer*>(param);

	while (!server->shutdown_)
	{
		server->ProcessNetworkQueue();

		server->ProcessDbResQueue();

		server->Update();

		TickController::Instance().Update();
	}

	return 0;
}

void MyServer::ProcessNetworkQueue()
{
	while (1)
	{
		Packet* packet = networkPacketQueue_.Pop();

		if (packet == nullptr)
			return;

		__int64 sessionId = packet->GetId();

		// 함수의 인자 자료형 주의
		if (!rpcStub_.PacketProc(sessionId, packet))
		{
			//disconnect(sessionId);

			return;
		}

		delete packet;
	}
}

void MyServer::Update()
{

}


// ----------------------------------------------------- //


unsigned int __stdcall MyServer::DatabaseThread(void* param)
{
	MyServer* server = static_cast<MyServer*>(param);

	while (!server->shutdown_)
	{
		server->ProcessDbReqQueue();

		Sleep(10);
	}

	return 0;
}

void MyServer::ProcessDbReqQueue()
{
	while (1)
	{
		Packet* packet = dbReqQueue_.Pop();

		if (packet == nullptr)
		{
			return;
		}

		__int64 sessionId = packet->GetId();

		dbStub_.DbPacketProc(sessionId, packet);

		delete packet;
	}
}

void MyServer::ProcessDbResQueue()
{
	while (1)
	{
		Packet* packet = dbResQueue_.Pop();

		if (packet == nullptr)
		{
			return;
		}

		__int64 sessionId = packet->GetId();

		dbStub_.DbPacketProc(sessionId, packet);

		delete packet;
	}
}


// ----------------------------------------------------- //
// RPC
// ----------------------------------------------------- //


bool MyServer::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		rpcProxy_.ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_.ReqUserRegisterDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		rpcProxy_.ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_.ReqUserLoginDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
	dbProxy_.ReqPlayerRegisterDB(sessionId, userId, playerName);

	return true;
}

bool MyServer::ReqPlayerProfile(__int64 sessionId, int userId)
{
	dbProxy_.ReqPlayerProfileDB(sessionId, userId);

	return true;
}

bool MyServer::ReqCharacterList(__int64 sessionId, int playerId)
{
	// 접속 중이 아님 or 로비 상태 아님
	if (playerMap_.count(playerId) == 0 || playerMap_[playerId]->state != PLAYER_STATE::LOBBY)
	{
		std::list<Character> lst;

		RESPONSE_CODE c = RESPONSE_CODE::PLAYER_NOT_CONNECTED;

		rpcProxy_.ResCharacterList(sessionId, c, lst);

		return true;
	}

	dbProxy_.ReqCharacterListDB(sessionId, playerId);

	return true;
}

bool MyServer::ReqPlayerList(__int64 sessionId)
{
	std::list<Player> playerList;

	for (auto& i : playerMap_)
	{
		Player p;
		p = *i.second;

		playerList.push_back(p);
	}

	rpcProxy_.ResPlayerList(sessionId, playerList);

	return true;
}

bool MyServer::ReqChat(__int64 sessionId, std::string& message)
{
	int senderId = sessionToPlayer_[sessionId];

	// 플레이어가 등록되지 않았다면 리턴
	if (playerMap_.count(senderId) == 0)
		return true;

	// 플레이어가 로비가 아니라면 리턴
	if ((playerMap_[senderId])->state != PLAYER_STATE::LOBBY)
		return true;

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		rpcProxy_.ResChat(other->sessionId_, senderId, message);
	}

	return true;
}

// ----------------------------------------------------- //
// DB
// ----------------------------------------------------- //


bool MyServer::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository_.Create(loginId, password);

	dbProxy_.ResUserRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	rpcProxy_.ResUserRegister(sessionId, code);

	return true;
}

bool MyServer::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	int userId = 0;

	RESPONSE_CODE code = userRepository_.FindByLoginId(loginId, password, userId);

	dbProxy_.ResUserLoginDB(sessionId, code, userId);

	return true;
}

bool MyServer::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
	rpcProxy_.ResUserLogin(sessionId, code, userId);

	return true;
}




bool MyServer::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
	RESPONSE_CODE code = playerRepository_.Create(userId, playerName);

	dbProxy_.ResPlayerRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	rpcProxy_.ResPlayerRegister(sessionId, code);

	return true;
}

bool MyServer::ReqPlayerProfileDB(__int64 sessionId, int userId)
{
	Player p;

	RESPONSE_CODE code = playerRepository_.FindByUserId(userId, p);

	dbProxy_.ResPlayerProfileDB(sessionId, code, p);

	return true;
}

bool MyServer::ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player)
{
	if (code != RESPONSE_CODE::SUCCESS)
	{
		rpcProxy_.ResPlayerProfile(sessionId, code, player);

		return true;
	}

	auto it = playerMap_.find(player.playerId_);

	// 이미 접속 중
	if (it != playerMap_.end())
	{
		RESPONSE_CODE c = RESPONSE_CODE::PLAYER_ALREADY_CONNECTED;

		rpcProxy_.ResPlayerProfile(sessionId, c, player);

		return true;
	}

	// 성공이라면 서버의 playerMap에 저장
	sessionToPlayer_.insert({ sessionId, player.playerId_ });

	Player* newPlayer = new Player();
	newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.level_, player.gold_);

	playerMap_.insert({ player.playerId_ , newPlayer });


	rpcProxy_.ResPlayerProfile(sessionId, code, player);

	return true;
}

bool MyServer::ReqCharacterListDB(__int64 sessionId, int playerId)
{
	std::list<Character> lst;

	RESPONSE_CODE code = inventoryRepository_.SelectCharacters(playerId, lst);

	dbProxy_.ResCharacterListDB(sessionId, code, lst);

	return true;
}

bool MyServer::ResCharacterListDB(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList)
{
	if (code != RESPONSE_CODE::SUCCESS)
	{
		rpcProxy_.ResCharacterList(sessionId, code, characterList);

		return true;
	}

	// 성공이라면 서버의 playerMap에 저장

	//Player* newPlayer = new Player();
	//newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.level_, player.gold_);



	//rpcProxy_.ResCharacterList(sessionId, code, player);

	return true;
}
