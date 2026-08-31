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

void DatabaseServerProxy::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 13;
    *packet << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerCharactersDB(__int64 sessionId, std::list<Character>& characters)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 14;
    *packet << characters;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResLobbyPlayersDB(__int64 sessionId, std::list<PlayerInfo>& players)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 15;
    *packet << players;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 17;
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

void DatabaseServerProxy::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 50;
    *packet << playerId << inventoryId << characterId << curGold;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ResBuyCharacterDB(__int64 sessionId, Character& character, int curGold)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 51;
    *packet << character << curGold;

    logicQueue_->Push(packet);
}

void DatabaseServerProxy::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 52;
    *packet << playerId << inventoryId;

    dbQueue_->Push(packet);
}

void DatabaseServerProxy::ReqEndGameDB(__int64 sessionId, int result, int currentMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 64;
    *packet << result << currentMoney;

    dbQueue_->Push(packet);
}
