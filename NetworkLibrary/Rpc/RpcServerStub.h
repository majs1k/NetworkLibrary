#pragma once

#include "../Utils/Packet.h"

class RpcServerStub
{
public:
	bool PacketProc(__int64 sessionId, Packet* packet)
	{
		switch (packet->GetHeaderPtr()->type_)
		{
		case 0: 
		{
			int id;

			*packet >> id;

			return this->LoginRequest(sessionId, id);
			break;
		}
		case 1: 
		{
			int id;

			*packet >> id;

			return this->LoginResponse(sessionId, id);
			break;
		}
		default:
		{
			// 로그 추가

			return false;
		}
		}
	}

	virtual bool LoginRequest(__int64 sessionId, int id)
	{
		return true;
	}
	virtual bool LoginResponse(__int64 sessionId, int id)
	{
		return true;
	}
};
