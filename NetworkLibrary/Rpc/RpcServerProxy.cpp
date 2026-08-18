#include "RpcServerProxy.h"


void RpcServerProxy::LoginRequest(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::LoginResponse(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ChatRequest(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << chat;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ChatResponse(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << chat;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ItemRequest(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ItemResponse(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}
