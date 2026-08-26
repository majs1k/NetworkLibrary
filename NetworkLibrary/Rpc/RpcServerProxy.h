#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
    LanServer* server_;

public:
    void ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserRegister(__int64 sessionId, RESPONSE_CODE code);
    void ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);
    void ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerEnterLobby(__int64 sessionId, int userId);
    void ResPlayerProfile(__int64 sessionId, Player player);
    void ResPlayerCharacters(__int64 sessionId, std::list<Character> characterList);
    void ReqChat(__int64 sessionId, std::string& message);
    void ResChat(__int64 sessionId, int playerId, std::string& message);
    void ReqLobbyPlayers(__int64 sessionId);
    void ResLobbyPlayers(__int64 sessionId, std::list<Player> playerList);
    void ResPlayerEnterLobby(__int64 sessionId, Player player);
    void ResPlayerLeaveLobby(__int64 sessionId, int playerId);
    void ReqBuyCharacter(__int64 sessionId);
    void ResBuyCharacter(__int64 sessionId, Character character, int currentMoney);
    void ReqEnterMatch(__int64 sessionId);
    void ResEnterMatch(__int64 sessionId);
    void ResStartMatch(__int64 sessionId);
};
