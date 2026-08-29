#include "MyServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"

#include "../RPC/RpcServerProxy.h"
#include "../Database/DatabaseServerProxy.h"

#include <process.h>
#include <Windows.h>


MyServer::MyServer()
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


	db_.Connect();

	userRepository_.Initialize(&db_);
	playerRepository_.Initialize(&db_);
	inventoryRepository_.Initialize(&db_);
}

MyServer::~MyServer()
{
	delete rpcProxy_;
	delete rpcStub_;

	delete dbProxy_;
	delete dbStub_;
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
	if (sessionToPlayer_.count(sessionId) == 0)
		return;

	int playerId = sessionToPlayer_[sessionId];

	playerMap_.erase(playerId);

	sessionToPlayer_.erase(sessionId);

	// 다른 플레이어들에게 퇴장 알림
	for (auto& i : playerMap_)
	{
		rpcProxy_->ResPlayerLeaveLobby(i.second->sessionId_, playerId);
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
	// 플레이어가 등록되지 않았다면 리턴
	if (sessionToPlayer_.count(sessionId) == 0)
		return true;

	int playerId = sessionToPlayer_[sessionId];

	//if (playerMap_.count(playerId) == 0)
	//	return true;

	// 플레이어가 로비가 아니라면 리턴
	if ((playerMap_[playerId])->state_ != PLAYER_STATE::LOBBY)
		return true;

	for (auto& p : playerMap_)
	{
		Player* other = p.second;

		rpcProxy_->ResChat(other->sessionId_, playerId, message);
	}

	return true;
}

bool MyServer::ReqBuyCharacter(__int64 sessionId)
{
	// 플레이어가 등록되지 않았다면 리턴
	if (sessionToPlayer_.count(sessionId) == 0)
		return true;

	int playerId = sessionToPlayer_[sessionId];

	Player* player = playerMap_[playerId];

	// 플레이어가 로비가 아니라면 리턴
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
	// 이것도 charinfo 따로 만들어야할듯..
	ch.characterId_ = 1;

	rpcProxy_->ResBuyCharacter(sessionId, ch, player->gold_);

	/// 잠만 이거 어케 전달해야함???
	dbProxy_->ReqBuyCharacterDB(sessionId, playerId, ch.characterId_, player->gold_);

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
	// 잘못된 요청 (플레이어 이미 접속)
	if (playerMap_.count(player.playerId_) != 0)
	{
		player.playerId_ = 0;

		rpcProxy_->ResPlayerProfile(sessionId, player);

		return true;
	}

	/// 플레이어 자료구조들은 로직스레드에서만 변경
	sessionToPlayer_.insert({ sessionId, player.playerId_ });

	Player* newPlayer = new Player();
	newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.level_, player.gold_);

	playerMap_.insert({ player.playerId_ , newPlayer });


	rpcProxy_->ResPlayerProfile(sessionId, player);


	// 플레이어에게 로비 플레이어들 정보 송신
	std::list<PlayerInfo> infos;

	for (auto& p : playerMap_)
	{
		PlayerInfo info(*(p.second));

		infos.push_back(info);
	}

	rpcProxy_->ResLobbyPlayers(sessionId, infos);

	// 복사생성자 호출
	PlayerInfo info(player);

	// 다른 플레이어들에게도 입장 알림
	for (auto& p : playerMap_)
	{
		rpcProxy_->ResPlayerEnterLobby(p.second->sessionId_, info);
	}

	return true;
}

bool MyServer::ResPlayerCharactersDB(__int64 sessionId, std::list<Character>& characters)
{
	int playerId = sessionToPlayer_[sessionId];

	Player* p = playerMap_[playerId];

	// 플레이어의 캐릭터 등록
	for (auto& c : characters)
	{
		p->characters_.push_back(c);
	}

	rpcProxy_->ResPlayerCharacters(sessionId, characters);

	return true;
}

bool MyServer::ReqBuyCharacterDB(__int64 sessionId, int playerId, int characterId, int curGold)
{
	///TODO: 트랜잭션 추가
	playerRepository_.UpdateGold(playerId, curGold);
	inventoryRepository_.CreateCharacter(playerId, characterId);
	
	return true;
}
