#include "RpcServerProxy.h"


void RpcServerProxy::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResUserRegister(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code)
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

void RpcServerProxy::ReqCharacterList(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << playerId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResCharacterList(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet << code << characterList;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqPlayerList(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerList(__int64 sessionId, std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 21;
    *packet << playerList;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerEnter(__int64 sessionId, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 22;
    *packet << player;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerLeave(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 23;
    *packet << playerId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerDelete(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 24;
    *packet << playerId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqChat(__int64 sessionId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << message;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChat(__int64 sessionId, int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << playerId << message;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqEnterRoom(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 50;
    *packet << roomId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << code << roomId << roomName << lst;

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
