#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/SendPacket.h"
#include "../Utils/RecvPacket.h"
#include "../Utils/Message.h"
#include "../Utils/TRingBuffer.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <process.h>
#include <Windows.h>

TestServer::TestServer()
{
	//proxy_.server_ = this;

	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, logicThread, this, 0, nullptr);

	// 종료시
	//shutdown_ = true;
}

bool TestServer::onConnectionRequest(const std::wstring& ip, int port)
{
	if (sessionCount() >= sessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void TestServer::onAccept(__int64 sessionId)
{

}

void TestServer::onRelease(__int64 sessionId)
{

}

void TestServer::onRecv(__int64 sessionId, Packet* packet)
{
	Message* message = new Message();
	message->setId(sessionId);
	message->setPacket(packet);

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

void TestServer::onRecv(__int64 sessionId, RecvPacket* packet)
{
	//MESSAGE message;
	//*packet >> message.data_;

	//Packet packet2;
	//packet2 << message.data_;

	//// 호출부 안에서 패킷 헤더를 삽입
	//this->sendPacket(sessionId, &packet2);
}

void TestServer::onError(int errorCode, wchar_t* str)
{

}

unsigned int __stdcall TestServer::logicThread(void* param)
{
	TestServer* server = (TestServer*)param;

	while (!server->shutdown_)
	{
		server->packetProc();

		server->update();

		TickController::getInstance().update();
	}

	return 0;
}

void TestServer::packetProc()
{
	while (1)
	{
		Message* message = messageQueue_.pop();

		if (message == nullptr)
			return;

		__int64 sessionId = message->sessionId();
		Packet* packet = message->packet();

		TEST_BODY bd;

		*packet >> bd.data_;

		delete packet;
		delete message;

		Packet packet2;
		packet2 << bd.data_;

		// 호출부 안에서 패킷 헤더를 삽입
		this->sendPacket(sessionId, &packet2);
	}
}

void TestServer::update()
{

}
