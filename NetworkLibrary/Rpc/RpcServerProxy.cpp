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

void RpcServerProxy::ResLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << code;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqPlayerProfile(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 13;
    *packet << code << player;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqRoomList(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResRoomList(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 21;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqRoomCreate(__int64 sessionId, std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 22;
    *packet << roomName;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResRoomCreate(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 23;
    *packet << code << roomId << roomName;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResRoomDelete(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 24;
    *packet << roomId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqEnterRoom(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 25;
    *packet << roomId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 26;
    *packet << code << roomId << roomName << lst;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterOtherUser(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 27;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqPlayerList(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 90;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerList(__int64 sessionId, std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 91;
    *packet << playerList;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqChat(__int64 sessionId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 100;
    *packet << message;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChat(__int64 sessionId, int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 101;
    *packet << playerId << message;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 200;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 201;
    *packet << lst;

    server_->SendPacket(sessionId, packet);
}
