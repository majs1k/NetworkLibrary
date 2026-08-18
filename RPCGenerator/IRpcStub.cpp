#include "IRpcStub.h"
#include "../Utils/Packet.h"

bool IRpcStub::PacketProc(__int64 sessionId, Packet* packet)
{
	switch (packet->GetHeaderPtr()->type_)
	{
	case 0: 
	{
		int num;

		*packet >> num;

		return this->cs_create_login(sessionId, num);
		break;
	}

	case 1: 
	{
		int num;

		*packet >> num;

		return this->sc_create_login(sessionId, num);
		break;
	}

	default:
	{
		// 로그 추가

		return false;
	}
	}
}

bool IRpcStub::cs_create_login(__int64 sessionId, int num)
{
	return true;
}

bool IRpcStub::sc_create_login(__int64 sessionId, int num)
{
	return true;
}
