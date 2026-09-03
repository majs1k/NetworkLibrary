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

	g_DbProxy.dbReqQueue_ = &dbReqQueue_;
	g_DbProxy.dbResQueue_ = &dbResQueue_;

	matchMaker_.Initialize(&roomManager_);

	roomManager_.SetPlayerManager(&playerManager_);
}

void MyServer::AttachProxy(RpcServerProxy* rpcProxy)
{
	rpcProxy->server_ = this;
}

void MyServer::AttachDb(Database* db)
{
	db_ = db;

	userRepository_.Initialize(db_);
	playerRepository_.Initialize(db_);
	inventoryRepository_.Initialize(db_);
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
	playerManager_.RemovePlayer(player);

	// 다른 플레이어들에게 퇴장 알림
	for (auto& p : playerManager_.GetPlayers())
	{
		g_RpcProxy.ResLeaveLobby(p.second->sessionId_, playerId);
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

	networkQueue_.Push(packet);
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
		Packet* packet = networkQueue_.Pop();

		if (packet == nullptr)
			return;

		__int64 sessionId = packet->GetId();

		// 함수의 인자 자료형 주의
		if (!PacketProc(sessionId, packet))
		{
			//Disconnect(sessionId);

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

		DbPacketProc(sessionId, packet);

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

		DbPacketProc(sessionId, packet);

		delete packet;
	}
}
