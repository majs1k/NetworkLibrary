#include "RpcClientProxy.h"


void RpcClientProxy::ReqRegister(std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResRegister(RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqLogin(std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResLogin(RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqCreatePlayer(int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResCreatePlayer(RESPONSE_CODE code)
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

void RpcClientProxy::ReqRoomList()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResRoomList()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 21;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqRoomCreate(std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 22;
    *packet << roomName;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResRoomCreate(RESPONSE_CODE code, int roomId, std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 23;
    *packet << code << roomId << roomName;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResRoomDelete(int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 24;
    *packet << roomId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqEnterRoom(int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 25;
    *packet << roomId;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 26;
    *packet << code << roomId << roomName << lst;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResEnterOtherUser()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 27;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqPlayerList()
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 90;
    *packet;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResPlayerList(std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 91;
    *packet << playerList;

    client_->SendPacket(packet);
}

void RpcClientProxy::ReqChat(std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 100;
    *packet << message;

    client_->SendPacket(packet);
}

void RpcClientProxy::ResChat(int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 101;
    *packet << playerId << message;

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
