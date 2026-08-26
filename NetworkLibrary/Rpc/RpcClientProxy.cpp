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

void RpcClientProxy::ReqPlayerProfile(int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerProfile(RESPONSE_CODE code, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 13;
    *packet << code << player;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqCharacterList(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 14;
    *packet << playerId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResCharacterList(RESPONSE_CODE code, std::list<Character> characterList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 15;
    *packet << code << characterList;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqPlayerList()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerList(std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 21;
    *packet << playerList;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerEnter(Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 22;
    *packet << player;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerLeave(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 23;
    *packet << playerId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerDelete(int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 24;
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

void RpcClientProxy::ReqEnterRoom(int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 50;
    *packet << roomId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 51;
    *packet << code << roomId << roomName << lst;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqUseItem(std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 200;
    *packet << lst;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResUseItem(std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 201;
    *packet << lst;

    client_->SendPacket(packet);
}
