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

void RpcServerProxy::ResPlayerCharacters(__int64 sessionId, std::vector<Character>& characters)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characters;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqEnterLobby(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterLobby(__int64 sessionId, PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResLeaveLobby(__int64 sessionId, int playerId)
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

void RpcServerProxy::ResBuyCharacter(__int64 sessionId, Character& character, int curMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << curMoney;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqChangeEquipment(__int64 sessionId, int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 52;
    *packet << inventoryId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChangeEquipment(__int64 sessionId, int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 53;
    *packet << inventoryId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqStartMatch(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 60;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResStartMatch(__int64 sessionId, STONE stone, PlayerInfo& opponent)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 61;
    *packet << stone << opponent;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqCancelMatch(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 62;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqPlaceStone(__int64 sessionId, short row, short col)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 65;
    *packet << row << col;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResPlaceStone(__int64 sessionId, short row, short col)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 66;
    *packet << row << col;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResGameResult(__int64 sessionId, STONE stone, int level, int myMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 67;
    *packet << stone << level << myMoney;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChangePlayerLevel(__int64 sessionId, int playerId, int level)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 68;
    *packet << playerId << level;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChangePlayerState(__int64 sessionId, int playerId, PLAYER_STATE state)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 69;
    *packet << playerId << state;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResEnterRoom(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 70;
    *packet << playerId;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ReqLeaveRoom(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 71;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResLeaveRoom(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 72;
    *packet << playerId;

    server_->SendPacket(sessionId, packet);
}
