#include "UnityServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <process.h>
#include <Windows.h>


UnityServer::UnityServer()
{
	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	hDatabaseThread_ = (HANDLE)_beginthreadex(nullptr, 0, DatabaseThread, this, 0, nullptr);

	rpc_.server_ = this;
	stub_.handler_ = this;

	dbProxy_.queue_ = &DatabaseReqQueue_;
	dbStub_.handler_ = this;
}

bool UnityServer::OnConnectionRequest(const std::wstring& ip, int port)
{
	if (SessionCount() >= SessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void UnityServer::OnAccept(__int64 sessionId)
{

}

void UnityServer::OnRelease(__int64 sessionId)
{

}

void UnityServer::OnRecv(__int64 sessionId, Packet* packet)
{
	//if (!stub_.PacketProc(sessionId, packet))
	//{
	//	//disconnect(sessionId);

	//	return;
	//}

	//delete packet;

	/// 로직 스레드에서 패킷 처리
	packet->SetId(sessionId);

	packetQueue_.Push(packet);
}

unsigned int __stdcall UnityServer::LogicThread(void* param)
{
	UnityServer* server = static_cast<UnityServer*>(param);

	while (!server->shutdown_)
	{
		server->ProcessNetPacket();

		server->Update();

		TickController::Instance().Update();
	}

	return 0;
}

void UnityServer::ProcessNetPacket()
{
	while (1)
	{
		Packet* packet = packetQueue_.Pop();

		if (packet == nullptr)
			return;

		__int64 sessionId = packet->GetId();

		// 함수의 인자 자료형 주의
		if (!stub_.PacketProc(sessionId, packet))
		{
			//disconnect(sessionId);

			return;
		}

		delete packet;
	}
}

void UnityServer::Update()
{

}



bool UnityServer::ReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{

	//rpc_.ResRegister(sessionId, RESPONSE_CODE::REGISTER_SUCCESS);
	dbProxy_.DbReqRegister(sessionId, loginId, password);


	return true;
}

bool UnityServer::ResRegister(__int64 sessionId, RESPONSE_CODE code)
{
	return true;
}

bool UnityServer::ReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
	dbProxy_.DbReqLogin(sessionId, loginId, password);

	return true;
}

bool UnityServer::ResLogin(__int64 sessionId, RESPONSE_CODE code)
{
	return true;
}


unsigned int __stdcall UnityServer::DatabaseThread(void* param)
{
	UnityServer* server = static_cast<UnityServer*>(param);

	while (!server->shutdown_)
	{
		Packet* packet = server->DatabaseReqQueue_.Pop();

		if (packet == nullptr)
		{
			Sleep(10);

			continue;
		}

		__int64 sessionId = packet->GetId();

		server->dbStub_.DbPacketProc(sessionId, packet);

		delete packet;
	}

	return 0;
}

bool UnityServer::DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository.Register(loginId, password);

	/// 수정요

	dbProxy_.DbResLogin(sessionId, code);

	return true;
}

bool UnityServer::DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
	userRepository.Register(loginId, password);

	return true;
}
