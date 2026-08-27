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

void DatabaseServerProxy::ReqPlayerEnterLobbyDB(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerProfileDB(__int64 sessionId, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 13;
    *packet << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerCharactersDB(__int64 sessionId, std::list<Character> characterList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characterList;

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

void DatabaseServerProxy::ReqLobbyPlayersDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 40;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResLobbyPlayersDB(__int64 sessionId, std::list<Player> playerList)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 41;
    *packet << playerList;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerEnterLobbyDB(__int64 sessionId, Player player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 42;
    *packet << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 43;
    *packet << playerId;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqBuyCharacterDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 50;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResBuyCharacterDB(__int64 sessionId, Character character, int currentMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << currentMoney;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqEnterMatchQueueDB(__int64 sessionId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 60;
    *packet;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResEnterMatchQueueDB(__int64 sessionId, Player otherPlayer)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 61;
    *packet << otherPlayer;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResEndMatchDB(__int64 sessionId, int result, int currentMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 62;
    *packet << result << currentMoney;

    logicQueue_->Push(packet);
}
