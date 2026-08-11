#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Message.h"
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
	//Message* message = new Message();
	//message->Initialize(sessionId, packet);

	//messageQueue_.push(message);

	/// IO 스레드에서 로직을 처리하는 방식
	__int64 data;
	*packet >> data;

	delete packet;

	SPacket* packet2 = new SPacket();
	packet2->Initialize();

	*packet2 << data;

	// 호출부 안에서 패킷 헤더를 삽입
	this->SendPacket(sessionId, packet2);
}

void TestServer::OnError(int errorCode, wchar_t* str)
{

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
		Message* message = messageQueue_.pop();

		if (message == nullptr)
			return;

		__int64 sessionId = message->sessionId_;
		Packet* packet = message->packet_;

		__int64 data;
		*packet >> data;

		delete packet;
		delete message;

		SPacket* packet2 = new SPacket();
		packet2->Initialize();

		*packet2 << data;

		// 호출부 안에서 패킷 헤더를 삽입
		this->SendPacket(sessionId, packet2);
	}
}

void TestServer::Update()
{

}
