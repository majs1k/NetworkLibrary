#include "MyServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"

#include "../RPC/RpcServerProxy.h"
#include "../Database/DatabaseServerProxy.h"

#include <process.h>
#include <Windows.h>


MyServer::MyServer(Database* db)
{
	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	hDatabaseThread_ = (HANDLE)_beginthreadex(nullptr, 0, DatabaseThread, this, 0, nullptr);

	rpcProxy_ = new RpcServerProxy();
	rpcStub_ = new RpcServerStub();

	rpcProxy_->server_ = this;
	rpcStub_->handler_ = this;

	dbProxy_ = new DatabaseServerProxy();
	dbStub_ = new DatabaseServerStub();


	dbProxy_->dbQueue_ = &dbReqQueue_;
	dbProxy_->logicQueue_ = &dbResQueue_;

	dbStub_->handler_ = this;

	db_ = db;

	userRepository_.Initialize(db_);
	playerRepository_.Initialize(db_);
	inventoryRepository_.Initialize(db_);

	matchMaker_.Initialize(&roomManager_);
}

MyServer::~MyServer()
{
	delete rpcProxy_;
	delete rpcStub_;

	delete dbProxy_;
	delete dbStub_;
}

bool MyServer::OnConnectionRequest(const std::string& ip, int port)
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
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return;

	int playerId = player->playerId_;

	// TODO: 인자 수정
	playerManager_.RemovePlayer(playerId);

	// 다른 플레이어들에게 퇴장 알림
	for (auto& p : playerManager_.GetPlayers())
	{
		rpcProxy_->ResPlayerLeaveLobby(p.second->sessionId_,playerId);
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

	// 로직 스레드에서 패킷 처리
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
		if (!rpcStub_->PacketProc(sessionId, packet))
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

		dbStub_->DbPacketProc(sessionId, packet);

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

		dbStub_->DbPacketProc(sessionId, packet);

		delete packet;
	}
}


// ----------------------------------------------------- //
// RPC
// ----------------------------------------------------- //


bool MyServer::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
	// 길이 제한
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		rpcProxy_->ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_->ReqUserRegisterDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		rpcProxy_->ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_->ReqUserLoginDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
	dbProxy_->ReqPlayerRegisterDB(sessionId, userId, playerName);

	return true;
}

bool MyServer::ReqPlayerConnection(__int64 sessionId, int userId)
{
	dbProxy_->ReqPlayerConnectionDB(sessionId, userId);

	return true;
}

bool MyServer::ReqChat(__int64 sessionId, std::string& message)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	for (auto& i : playerManager_.GetPlayers())
	{
		rpcProxy_->ResChat(			i.second->sessionId_,player->playerId_,message);
	}

	return true;
}

bool MyServer::ReqBuyCharacter(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	int playerId = player->playerId_;

	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	Character ch{};

	if (player->gold_ < 1000)
	{
		Character ch{};
		ch.characterId_ = 0;

		rpcProxy_->ResBuyCharacter(sessionId, ch,player->gold_);

		return true;
	}

	player->gold_ -= 1000;

	// TODO: 캐릭터 임의 생성 수정
	ch.characterId_ = 1;
	ch.inventoryId_ = inventoryRepository_.GenerateInventoryId();

	rpcProxy_->ResBuyCharacter(sessionId, ch, player->gold_);

	dbProxy_->ReqBuyCharacterDB(sessionId, playerId, ch.inventoryId_, ch.characterId_, player->gold_);

	return true;
}

bool MyServer::ReqChangeEquipment(__int64 sessionId, int inventoryId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	// 플레이어가 로비가 아니라면 리턴
	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	// 해당 캐릭터 id를 보유하고 있을시, db 저장
	for (auto& c : player->characters_)
	{
		if (c.inventoryId_ == inventoryId)
		{
			rpcProxy_->ResChangeEquipment(sessionId, inventoryId);

			dbProxy_->ReqChangeEquipmentDB(sessionId, player->playerId_, inventoryId);

			break;
		}
	}

	return true;
}

bool MyServer::ReqStartGame(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	matchMaker_.Add(player);

	return true;
}



// ----------------------------------------------------- //
// DB
// ----------------------------------------------------- //


bool MyServer::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository_.CreateUser(loginId, password);

	dbProxy_->ResUserRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	rpcProxy_->ResUserRegister(sessionId, code);

	return true;
}

bool MyServer::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	int userId = 0;

	RESPONSE_CODE code = userRepository_.FindByLoginId(loginId, password, userId);

	dbProxy_->ResUserLoginDB(sessionId, code, userId);

	return true;
}

bool MyServer::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
	rpcProxy_->ResUserLogin(sessionId, code, userId);

	return true;
}


bool MyServer::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
	RESPONSE_CODE code = playerRepository_.CreatePlayer(userId, playerName);

	dbProxy_->ResPlayerRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	rpcProxy_->ResPlayerRegister(sessionId, code);

	return true;
}

bool MyServer::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
	Player p;
	//TODO: 클라이언트 측에선 playerId 0으로 수신시 오류메세지 띄우기
	p.playerId_ = 0;

	playerRepository_.FindPlayerByUserId(userId, p);

	// 잘못된 요청 (해당 유저아이디의 플레이어 없음)
	if (p.playerId_ == 0)
	{
		rpcProxy_->ResPlayerProfile(sessionId, p);

		return true;
	}

	dbProxy_->ResPlayerProfileDB(sessionId, p);


	std::list<Character> lst;

	inventoryRepository_.FindCharacterByPlayerId(p.playerId_, lst);

	dbProxy_->ResPlayerCharactersDB(sessionId, lst);


	return true;
}

bool MyServer::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
	// 잘못된 요청 (플레이어 이미 서버 접속)
	if (playerManager_.ContainsPlayer(player.playerId_))
	{
		player.playerId_ = 0;

		rpcProxy_->ResPlayerProfile(sessionId, player);

		return true;
	}

	Player* newPlayer = new Player();
	newPlayer->Initialize(this->rpcProxy_, sessionId, player.playerId_, player.playerName_, player.level_, player.gold_, player.equippedInvenId_);

	if (!playerManager_.AddPlayer(sessionId, newPlayer))
	{
		delete newPlayer;

		// 클라이언트는 해당 패킷 수신시, 플레이어 등록 안되었음을 확인
		player.playerId_ = 0;

		rpcProxy_->ResPlayerProfile(sessionId, player);

		return true;
	}

	rpcProxy_->ResPlayerProfile(sessionId, player);


	// 플레이어에게 로비 플레이어들 정보 송신
	std::list<PlayerInfo> infos;

	for (auto& p: playerManager_.GetPlayers())
	{
		PlayerInfo info(*(p.second));
		infos.push_back(info);
	}

	rpcProxy_->ResLobbyPlayers(sessionId, infos);

	// 복사생성자 호출
	PlayerInfo info(player);

	// 다른 플레이어들에게도 입장 알림
	for (auto& p : playerManager_.GetPlayers())
	{
		rpcProxy_->ResPlayerEnterLobby(p.second->sessionId_, info);
	}

	return true;
}

bool MyServer::ResPlayerCharactersDB(__int64 sessionId, std::list<Character>& characters)
{
	Player* p = playerManager_.GetPlayerBySessionId(sessionId);

	if (p == nullptr)
		return true;

	// 플레이어의 캐릭터 등록
	for (auto& c : characters)
	{
		// TODO: emplace_back???
		p->characters_.push_back(c);
	}

	rpcProxy_->ResPlayerCharacters(sessionId, characters);

	return true;
}

bool MyServer::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold)
{
	db_-> BeginTransaction();

	playerRepository_.UpdateGold(playerId, curGold);
	inventoryRepository_.CreateCharacter(playerId, inventoryId, characterId);

	db_->Commit();
	
	return true;
}

bool MyServer::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
	playerRepository_.UpdateEquipment(playerId, inventoryId);

	return true;
}
