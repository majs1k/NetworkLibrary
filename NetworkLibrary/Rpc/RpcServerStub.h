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
    virtual bool ReqEnterLobby(__int64 sessionId);
    virtual bool ResEnterLobby(__int64 sessionId, PlayerInfo& player);
    virtual bool ResLeaveLobby(__int64 sessionId, int playerId);
    virtual bool ReqChat(__int64 sessionId, std::string& message);
    virtual bool ResChat(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqBuyCharacter(__int64 sessionId);
    virtual bool ResBuyCharacter(__int64 sessionId, Character& character, int curMoney);
    virtual bool ReqChangeEquipment(__int64 sessionId, int inventoryId);
    virtual bool ResChangeEquipment(__int64 sessionId, int inventoryId);
    virtual bool ReqStartMatch(__int64 sessionId);
    virtual bool ResStartMatch(__int64 sessionId, STONE stone, PlayerInfo& opponent);
    virtual bool ReqCancelMatch(__int64 sessionId);
    virtual bool ReqPlaceStone(__int64 sessionId, short row, short col);
    virtual bool ResPlaceStone(__int64 sessionId, short row, short col);
    virtual bool ResGameResult(__int64 sessionId, STONE stone, int level, int myMoney);
    virtual bool ResChangePlayerLevel(__int64 sessionId, int playerId, int level);
    virtual bool ResChangePlayerState(__int64 sessionId, int playerId, PLAYER_STATE state);
    virtual bool ResEnterRoom(__int64 sessionId, int playerId);
    virtual bool ReqLeaveRoom(__int64 sessionId);
    virtual bool ResLeaveRoom(__int64 sessionId, int playerId);
};
