#include "DatabaseServerProxy.h"


void DatabaseServerProxy::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
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

void DatabaseServerProxy::ReqCharacterListDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 14;
    *packet << playerId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResCharacterListDB(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 15;
    *packet << code << characterList;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqPlayerListDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 20;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerListDB(__int64 sessionId, std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 21;
    *packet << playerList;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerEnterDB(__int64 sessionId, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 22;
    *packet << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerLeaveDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 23;
    *packet << playerId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerDeleteDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 24;
    *packet << playerId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqChatDB(__int64 sessionId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 30;
    *packet << message;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResChatDB(__int64 sessionId, int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 31;
    *packet << playerId << message;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqEnterRoomDB(__int64 sessionId, int roomId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 50;
    *packet << roomId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 51;
    *packet << code << roomId << roomName << lst;

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
