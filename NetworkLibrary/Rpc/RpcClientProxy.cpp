#include "RpcClientProxy.h"


void RpcClientProxy::LoginRequest(int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::LoginResponse(int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ChatRequest(std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << chat;

    client_->SendPacket(packet);
}

void RpcClientProxy::ChatResponse(std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << chat;

    client_->SendPacket(packet);
}

void RpcClientProxy::ItemRequest(std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << lst;

    client_->SendPacket(packet);
}

void RpcClientProxy::ItemResponse(std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << lst;

    client_->SendPacket(packet);
}
