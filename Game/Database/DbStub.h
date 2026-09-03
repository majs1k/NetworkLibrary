#pragma once

#include "../Utils/Packet.h"

class DbStub
{
public:

    bool DbPacketProc(__int64 sessionId, Packet* packet);
    
    virtual bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerConnectionDB(__int64 sessionId, int userId);
    virtual bool ResPlayerProfileDB(__int64 sessionId, Player& player);
    virtual bool ResPlayerIconsDB(__int64 sessionId, std::vector<Icon>& icons);
    virtual bool ResLobbyPlayersDB(__int64 sessionId, std::vector<PlayerInfo>& players);
    virtual bool ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player);
    virtual bool ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId);
    virtual bool ReqChatDB(__int64 sessionId, std::string& message);
    virtual bool ResChatDB(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqBuyIconDB(__int64 sessionId, int playerId, int inventoryId, int iconId, int curMoney);
    virtual bool ResBuyIconDB(__int64 sessionId, Icon& icon, int curMoney);
    virtual bool ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId);
    virtual bool ReqGameResultDB(__int64 sessionId, int winnerId, int loserId);
};
