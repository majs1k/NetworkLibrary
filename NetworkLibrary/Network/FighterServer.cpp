#include "FighterServer.h"
#include "../Contents/Player.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"

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

void FighterServer::onRecv(__int64 sessionId, Packet& packet)
{

}

void FighterServer::onError(int errorCode, wchar_t* str)
{

}
