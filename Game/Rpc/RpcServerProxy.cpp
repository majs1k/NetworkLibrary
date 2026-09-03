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

void RpcServerProxy::ResPlayerIcons(__int64 sessionId, std::vector<Icon>& icons)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << icons;

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

void RpcServerProxy::ReqBuyIcon(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 50;
    *packet;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResBuyIcon(__int64 sessionId, Icon& icon, int curMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << icon << curMoney;

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

void RpcServerProxy::ResGameResult(__int64 sessionId, STONE winner, int myRating, int opponentRating, int myMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 67;
    *packet << winner << myRating << opponentRating << myMoney;

    server_->SendPacket(sessionId, packet);
}

void RpcServerProxy::ResChangePlayerRating(__int64 sessionId, int playerId, int rating)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 68;
    *packet << playerId << rating;

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
