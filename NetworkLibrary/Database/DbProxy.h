#pragma once

#include "../Utils/PacketQueue.h"
#include "../Utils/Packet.h"

class DbProxy
{
public:
    PacketQueue* dbReqQueue_;
    PacketQueue* dbResQueue_;

public:
    void ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);
    void ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerConnectionDB(__int64 sessionId, int userId);
    void ResPlayerProfileDB(__int64 sessionId, Player& player);
    void ResPlayerCharactersDB(__int64 sessionId, std::vector<Character>& characters);
    void ResLobbyPlayersDB(__int64 sessionId, std::vector<PlayerInfo>& players);
    void ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player);
    void ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId);
    void ReqChatDB(__int64 sessionId, std::string& message);
    void ResChatDB(__int64 sessionId, int playerId, std::string& message);
    void ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curMoney);
    void ResBuyCharacterDB(__int64 sessionId, Character& character, int curMoney);
    void ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId);
    void ReqGameResultDB(__int64 sessionId, int winnerId, int loserId);
};
