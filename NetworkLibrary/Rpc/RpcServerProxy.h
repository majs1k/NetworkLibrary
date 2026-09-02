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
    void ReqPlayerConnection(__int64 sessionId, int userId);
    void ResPlayerProfile(__int64 sessionId, Player& player);
    void ResPlayerCharacters(__int64 sessionId, std::vector<Character>& characters);
    void ResLobbyPlayers(__int64 sessionId, std::vector<PlayerInfo>& players);
    void ResPlayerEnterLobby(__int64 sessionId, PlayerInfo& player);
    void ResPlayerLeaveLobby(__int64 sessionId, int playerId);
    void ReqChat(__int64 sessionId, std::string& message);
    void ResChat(__int64 sessionId, int playerId, std::string& message);
    void ReqBuyCharacter(__int64 sessionId);
    void ResBuyCharacter(__int64 sessionId, Character& character, int curGold);
    void ReqChangeEquipment(__int64 sessionId, int inventoryId);
    void ResChangeEquipment(__int64 sessionId, int inventoryId);
    void ReqStartGame(__int64 sessionId);
    void ResStartGame(__int64 sessionId, PlayerInfo& otherPlayer);
    void ReqCancelGame(__int64 sessionId);
    void ReqGameCommand(__int64 sessionId, int commandType);
    void ResGameCommand(__int64 sessionId, Character& character);
    void ResEndGame(__int64 sessionId, int result, int currentMoney);
};
