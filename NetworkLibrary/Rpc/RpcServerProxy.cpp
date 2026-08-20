#include "RpcServerProxy.h"


void RpcServerProxy::ReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResRegister(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResLogin(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqChat(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << chat;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChat(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << chat;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}
