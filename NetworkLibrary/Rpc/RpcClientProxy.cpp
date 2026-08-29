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

void RpcClientProxy::ResPlayerCharacters(std::list<Character>& characters)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characters;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResLobbyPlayers(std::list<PlayerInfo>& players)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet << players;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerEnterLobby(PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerLeaveLobby(int playerId)
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

void RpcClientProxy::ResBuyCharacter(Character& character, int curGold)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << curGold;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqEnterMatchQueue()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 60;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterMatchQueue(PlayerInfo& otherPlayer)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 61;
    *packet << otherPlayer;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEndMatch(int result, int currentMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 62;
    *packet << result << currentMoney;

    client_->SendPacket(packet);
}
