#pragma once

#include "../Utils/Packet.h"

class RpcServerStub
{
public:

    bool PacketProc(__int64 sessionId, Packet* packet);
    
    virtual bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerConnection(__int64 sessionId, int userId);
    virtual bool ResPlayerProfile(__int64 sessionId, Player& player);
    virtual bool ResPlayerCharacters(__int64 sessionId, std::vector<Character>& characters);
    virtual bool ResLobbyPlayers(__int64 sessionId, std::vector<PlayerInfo>& players);
    virtual bool ResPlayerEnterLobby(__int64 sessionId, PlayerInfo& player);
    virtual bool ResPlayerLeaveLobby(__int64 sessionId, int playerId);
    virtual bool ReqChat(__int64 sessionId, std::string& message);
    virtual bool ResChat(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqBuyCharacter(__int64 sessionId);
    virtual bool ResBuyCharacter(__int64 sessionId, Character& character, int curGold);
    virtual bool ReqChangeEquipment(__int64 sessionId, int inventoryId);
    virtual bool ResChangeEquipment(__int64 sessionId, int inventoryId);
    virtual bool ReqStartGame(__int64 sessionId);
    virtual bool ResStartGame(__int64 sessionId, PlayerInfo& otherPlayer);
    virtual bool ReqCancelGame(__int64 sessionId);
    virtual bool ReqGameCommand(__int64 sessionId, int commandType);
    virtual bool ResGameCommand(__int64 sessionId, Character& character);
    virtual bool ResEndGame(__int64 sessionId, int result, int currentMoney);
};
