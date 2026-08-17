#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <process.h>
#include <Windows.h>

TestServer::TestServer()
{
	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	// 종료시
	//shutdown_ = true;
}

bool TestServer::OnConnectionRequest(const std::wstring& ip, int port)
{
	if (SessionCount() >= SessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void TestServer::OnAccept(__int64 sessionId)
{

}

void TestServer::OnRelease(__int64 sessionId)
{

}

void TestServer::OnRecv(__int64 sessionId, Packet* packet)
{
	/// IO 스레드에서 패킷 처리
	__int64 data;
	*packet >> data;

	delete packet;

	Packet* packet2 = new Packet();
	packet2->Initialize();

	*packet2 << data;

	this->SendPacket(sessionId, packet2);


	/// 로직 스레드에서 패킷 처리
	//packet->SetId(sessionId);
	//
	//packetQueue_.push(packet);
}

unsigned int __stdcall TestServer::LogicThread(void* param)
{
	TestServer* server = (TestServer*)param;

	while (!server->shutdown_)
	{
		server->PacketProc();

		server->Update();

		TickController::Instance().Update();
	}

	return 0;
}

void TestServer::PacketProc()
{
	while (1)
	{
		Packet* packet = packetQueue_.pop();

		if (packet == nullptr)
			return;

		__int64 sessionId = packet->GetId();

		__int64 data;
		*packet >> data;

		delete packet;

		Packet* packet2 = new Packet();
		packet2->Initialize();

		*packet2 << data;

		this->SendPacket(sessionId, packet2);
	}
}

void TestServer::Update()
{

}
