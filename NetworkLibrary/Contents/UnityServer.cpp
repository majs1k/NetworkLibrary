#include "UnityServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/TickController.h"
#include <process.h>
#include <Windows.h>

UnityServer::UnityServer()
{
	hLogicThread_ = (HANDLE)_beginthreadex(nullptr, 0, LogicThread, this, 0, nullptr);

	proxy_.server_ = this;

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
	// 함수의 인자 자료형 주의
	if (!PacketProc(sessionId, packet))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}

unsigned int __stdcall UnityServer::LogicThread(void* param)
{
	UnityServer* server = (UnityServer*)param;

	while (!server->shutdown_)
	{
		//PacketProc();

		server->Update();

		TickController::Instance().Update();
	}

	return 0;
}

void UnityServer::Update()
{

}


bool UnityServer::cs_create_login(__int64 sessionId, int num)
{
	proxy_.cs_create_login(sessionId, num);


	return true;
}

bool UnityServer::sc_create_login(__int64 sessionId, int id, int num)
{
	///echo
	std::cout << num << std::endl;

	return true;
}
