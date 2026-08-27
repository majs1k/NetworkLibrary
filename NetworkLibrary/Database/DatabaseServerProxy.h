#pragma once

#include "../Utils/PacketQueue.h"
#include "../Utils/Packet.h"

class DatabaseServerProxy
{
public:
    PacketQueue* logicQueue_;
    PacketQueue* dbQueue_;

public:
    void ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);
    void ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerEnterLobbyDB(__int64 sessionId, int userId);
    void ResPlayerProfileDB(__int64 sessionId, Player player);
    void ResPlayerCharactersDB(__int64 sessionId, std::list<Character> characterList);
    void ReqChatDB(__int64 sessionId, std::string& message);
    void ResChatDB(__int64 sessionId, int playerId, std::string& message);
    void ReqLobbyPlayersDB(__int64 sessionId);
    void ResLobbyPlayersDB(__int64 sessionId, std::list<Player> playerList);
    void ResPlayerEnterLobbyDB(__int64 sessionId, Player player);
    void ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId);
    void ReqBuyCharacterDB(__int64 sessionId);
    void ResBuyCharacterDB(__int64 sessionId, Character character, int currentMoney);
    void ReqEnterMatchQueueDB(__int64 sessionId);
    void ResEnterMatchQueueDB(__int64 sessionId, Player otherPlayer);
    void ResEndMatchDB(__int64 sessionId, int result, int currentMoney);
};
