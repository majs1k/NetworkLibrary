#include "FighterServer.h"
#include "../Contents/Player.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../RPC/RPCStub.h"

bool FighterServer::onConnectionRequest(const std::wstring& ip, int port)
{
	if (sessionCount_ >= sessionMax_)
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	{
		// TODO: lan 외부 ip / 해외 ip / DDos 공격 차단
	}

	return true;
}

void FighterServer::onAccept(__int64 sessionId)
{
	PlayerManager::getInstance().createPlayer(sessionId);
}

void FighterServer::onRelease(__int64 sessionId)
{
	PlayerManager::getInstance().removePlayer(sessionId);
}

#include <iostream>

void FighterServer::onRecv(__int64 sessionId, Packet& packet)
{
	InterlockedIncrement(&recvMessageCount_);

	unsigned char type;
	packet >> type;

	// 함수의 인자 자료형 주의 필요 (이후 패킷헤더 수정시 참고)
	if (!RPCStub::getInstance().packetProc(sessionId, packet, type))
	{
		/// IOcount 감소시키는걸로 수정해야 할거 같은데...
		disconnect(sessionId);

		return;
	}
}

void FighterServer::onError(int errorCode, wchar_t* str)
{

}
