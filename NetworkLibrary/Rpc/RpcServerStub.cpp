#include "RpcServerStub.h"


bool RpcServerStub::PacketProc(__int64 sessionId, Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			int userId;

            *packet >> userId;

            return this->LoginRequest(sessionId, userId);
        }
        case 1:
        {
			int userId;

            *packet >> userId;

            return this->LoginResponse(sessionId, userId);
        }
        case 10:
        {
			std::string chat;

            *packet >> chat;

            return this->ChatRequest(sessionId, chat);
        }
        case 11:
        {
			std::string chat;

            *packet >> chat;

            return this->ChatResponse(sessionId, chat);
        }
        case 30:
        {
			std::list<int> lst;

            *packet >> lst;

            return this->ItemRequest(sessionId, lst);
        }
        case 31:
        {
			std::list<int> lst;

            *packet >> lst;

            return this->ItemResponse(sessionId, lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcServerStub::LoginRequest(__int64 sessionId, int userId)
{
    return true;
}

bool RpcServerStub::LoginResponse(__int64 sessionId, int userId)
{
    return true;
}

bool RpcServerStub::ChatRequest(__int64 sessionId, std::string& chat)
{
    return true;
}

bool RpcServerStub::ChatResponse(__int64 sessionId, std::string& chat)
{
    return true;
}

bool RpcServerStub::ItemRequest(__int64 sessionId, std::list<int>& lst)
{
    return true;
}

bool RpcServerStub::ItemResponse(__int64 sessionId, std::list<int>& lst)
{
    return true;
}
