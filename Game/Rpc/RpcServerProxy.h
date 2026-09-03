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
    void ResPlayerIcons(__int64 sessionId, std::vector<Icon>& icons);
    void ReqEnterLobby(__int64 sessionId);
    void ResEnterLobby(__int64 sessionId, PlayerInfo& player);
    void ResLeaveLobby(__int64 sessionId, int playerId);
    void ReqChat(__int64 sessionId, std::string& message);
    void ResChat(__int64 sessionId, int playerId, std::string& message);
    void ReqBuyIcon(__int64 sessionId);
    void ResBuyIcon(__int64 sessionId, Icon& icon, int curMoney);
    void ReqChangeEquipment(__int64 sessionId, int inventoryId);
    void ResChangeEquipment(__int64 sessionId, int inventoryId);
    void ReqStartMatch(__int64 sessionId);
    void ResStartMatch(__int64 sessionId, STONE stone, PlayerInfo& opponent);
    void ReqCancelMatch(__int64 sessionId);
    void ReqPlaceStone(__int64 sessionId, short row, short col);
    void ResPlaceStone(__int64 sessionId, short row, short col);
    void ResGameResult(__int64 sessionId, STONE winner, int myRating, int opponentRating, int myMoney);
    void ResChangePlayerRating(__int64 sessionId, int playerId, int rating);
    void ResChangePlayerState(__int64 sessionId, int playerId, PLAYER_STATE state);
    void ResEnterRoom(__int64 sessionId, int playerId);
    void ReqLeaveRoom(__int64 sessionId);
    void ResLeaveRoom(__int64 sessionId, int playerId);
};
