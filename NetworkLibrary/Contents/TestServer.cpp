#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/RecvPacket.h"
#include "../Utils/Message.h"
#include "../Utils/TRingBuffer.h"
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
	Message* message = new Message();
	message->Initialize(sessionId, packet);

	messageQueue_.push(message);


	//DUMMY_PACKET dum;
	//*packet >> dum.data_;

	//delete packet;

	//Packet packet2;
	//packet2 << dum.data_;

	//// 호출부 안에서 패킷 헤더를 삽입
	//this->sendPacket(sessionId, &packet2);


	/// 여러 세션으로 send
	//this->sendPacketSkipCopy(sessionId, p);
}

void TestServer::OnRecv(__int64 sessionId, RecvPacket* packet)
{
	//MESSAGE message;
	//*packet >> message.data_;

	//Packet packet2;
	//packet2 << message.data_;

	//// 호출부 안에서 패킷 헤더를 삽입
	//this->sendPacket(sessionId, &packet2);
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


		Packet* packet2 = new Packet();
		*packet2 << data;

		// 호출부 안에서 패킷 헤더를 삽입
		this->SendPacket(sessionId, packet2);
	}
}

void TestServer::Update()
{

}
