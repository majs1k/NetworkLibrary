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

void RpcServerProxy::ReqPlayerConnection(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerProfile(__int64 sessionId, Player& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 13;
    *packet << player;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerCharacters(__int64 sessionId, std::list<Character>& characters)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characters;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResLobbyPlayers(__int64 sessionId, std::list<PlayerInfo>& players)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet << players;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerEnterLobby(__int64 sessionId, PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlayerLeaveLobby(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 17;
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

void RpcServerProxy::ReqBuyCharacter(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 50;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResBuyCharacter(__int64 sessionId, Character& character, int curGold)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << curGold;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqEnterMatchQueue(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 60;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterMatchQueue(__int64 sessionId, PlayerInfo& otherPlayer)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 61;
    *packet << otherPlayer;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEndMatch(__int64 sessionId, int result, int currentMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 62;
    *packet << result << currentMoney;

    server_->SendPacket(sessionId, packet);
}
