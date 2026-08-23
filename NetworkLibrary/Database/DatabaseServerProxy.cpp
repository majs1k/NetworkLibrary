#include "DatabaseServerProxy.h"


void DatabaseServerProxy::ReqRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqCreatePlayerDB(__int64 sessionId, int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResCreatePlayerDB(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 11;
    *packet << code;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqPlayerProfileDB(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 13;
    *packet << code << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqRoomListDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResRoomListDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 21;
    *packet;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqRoomCreateDB(__int64 sessionId, std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 22;
    *packet << roomName;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResRoomCreateDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 23;
    *packet << code << roomId << roomName;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResRoomDeleteDB(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 24;
    *packet << roomId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqEnterRoomDB(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 25;
    *packet << roomId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 26;
    *packet << code << roomId << roomName << lst;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResEnterOtherUserDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 27;
    *packet;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqPlayerListDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 90;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerListDB(__int64 sessionId, std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 91;
    *packet << playerList;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqChatDB(__int64 sessionId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 100;
    *packet << message;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResChatDB(__int64 sessionId, int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 101;
    *packet << playerId << message;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqUseItemDB(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 200;
    *packet << lst;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResUseItemDB(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 201;
    *packet << lst;

    logicQueue_->Push(packet);
}
