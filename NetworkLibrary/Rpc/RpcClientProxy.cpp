#include "RpcClientProxy.h"


void RpcClientProxy::ReqUserRegister(std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResUserRegister(RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqUserLogin(std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResUserLogin(RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqPlayerRegister(int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerRegister(RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << code;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqPlayerConnection(int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerProfile(Player& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 13;
    *packet << player;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerCharacters(std::vector<Character>& characters)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characters;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqEnterLobby()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterLobby(PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResLeaveLobby(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 17;
    *packet << playerId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqChat(std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << message;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResChat(int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << playerId << message;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqBuyCharacter()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 50;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResBuyCharacter(Character& character, int curMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << curMoney;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqChangeEquipment(int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 52;
    *packet << inventoryId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResChangeEquipment(int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 53;
    *packet << inventoryId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqStartMatch()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 60;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResStartMatch(STONE stone, PlayerInfo& opponent)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 61;
    *packet << stone << opponent;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqCancelMatch()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 62;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqPlaceStone(short row, short col)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 65;
    *packet << row << col;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlaceStone(short row, short col)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 66;
    *packet << row << col;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResGameResult(STONE stone, int level, int myMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 67;
    *packet << stone << level << myMoney;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResChangePlayerLevel(int playerId, int level)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 68;
    *packet << playerId << level;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResChangePlayerState(int playerId, PLAYER_STATE state)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 69;
    *packet << playerId << state;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterRoom(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 70;
    *packet << playerId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqLeaveRoom()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 71;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResLeaveRoom(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 72;
    *packet << playerId;

    client_->SendPacket(packet);
}
