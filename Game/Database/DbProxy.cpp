#include "DbProxy.h"


void DbProxy::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code << userId;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 10;
    *packet << userId << playerName;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 11;
    *packet << code;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 12;
    *packet << userId;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 13;
    *packet << player;

    dbResQueue_->Push(packet);
}

void DbProxy::ResPlayerIconsDB(__int64 sessionId, std::vector<Icon>& icons)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 14;
    *packet << icons;

    dbResQueue_->Push(packet);
}

void DbProxy::ResLobbyPlayersDB(__int64 sessionId, std::vector<PlayerInfo>& players)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 15;
    *packet << players;

    dbResQueue_->Push(packet);
}

void DbProxy::ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 16;
    *packet << player;

    dbResQueue_->Push(packet);
}

void DbProxy::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 17;
    *packet << playerId;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqChatDB(__int64 sessionId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 30;
    *packet << message;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResChatDB(__int64 sessionId, int playerId, std::string& message)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 31;
    *packet << playerId << message;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqBuyIconDB(__int64 sessionId, int playerId, int inventoryId, int iconId, int curMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 50;
    *packet << playerId << inventoryId << iconId << curMoney;

    dbReqQueue_->Push(packet);
}

void DbProxy::ResBuyIconDB(__int64 sessionId, Icon& icon, int curMoney)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 51;
    *packet << icon << curMoney;

    dbResQueue_->Push(packet);
}

void DbProxy::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 52;
    *packet << playerId << inventoryId;

    dbReqQueue_->Push(packet);
}

void DbProxy::ReqGameResultDB(__int64 sessionId, int winnerId, int loserId)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);

    packet->GetHeaderPtr()->type_ = 67;
    *packet << winnerId << loserId;

    dbReqQueue_->Push(packet);
}
