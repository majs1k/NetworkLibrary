#include "RpcClientStub.h"


bool RpcClientStub::PacketProc(Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			int userId;

            *packet >> userId;

            return this->LoginRequest(userId);
        }
        case 1:
        {
			int userId;

            *packet >> userId;

            return this->LoginResponse(userId);
        }
        case 10:
        {
			std::string chat;

            *packet >> chat;

            return this->ChatRequest(chat);
        }
        case 11:
        {
			std::string chat;

            *packet >> chat;

            return this->ChatResponse(chat);
        }
        case 30:
        {
			std::list<int> lst;

            *packet >> lst;

            return this->ItemRequest(lst);
        }
        case 31:
        {
			std::list<int> lst;

            *packet >> lst;

            return this->ItemResponse(lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcClientStub::LoginRequest(int userId)
{
    return true;
}

bool RpcClientStub::LoginResponse(int userId)
{
    return true;
}

bool RpcClientStub::ChatRequest(std::string& chat)
{
    return true;
}

bool RpcClientStub::ChatResponse(std::string& chat)
{
    return true;
}

bool RpcClientStub::ItemRequest(std::list<int>& lst)
{
    return true;
}

bool RpcClientStub::ItemResponse(std::list<int>& lst)
{
    return true;
}
