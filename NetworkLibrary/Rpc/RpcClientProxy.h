#pragma once

#include "../Network/LanClient.h"
#include "../Utils/Packet.h"

class RpcClientProxy
{
public:
    LanClient* client_;

public:
    void ReqUserRegister(std::string& loginId, std::string& password);
    void ResUserRegister(RESPONSE_CODE code);
    void ReqUserLogin(std::string& loginId, std::string& password);
    void ResUserLogin(RESPONSE_CODE code, int userId);
    void ReqPlayerRegister(int userId, std::string& playerName);
    void ResPlayerRegister(RESPONSE_CODE code);
    void ReqPlayerConnection(int userId);
    void ResPlayerProfile(Player& player);
    void ResPlayerCharacters(std::vector<Character>& characters);
    void ReqEnterLobby();
    void ResEnterLobby(PlayerInfo& player);
    void ResLeaveLobby(int playerId);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqBuyCharacter();
    void ResBuyCharacter(Character& character, int curMoney);
    void ReqChangeEquipment(int inventoryId);
    void ResChangeEquipment(int inventoryId);
    void ReqStartMatch();
    void ResStartMatch(STONE stone, PlayerInfo& opponent);
    void ReqCancelMatch();
    void ReqPlaceStone(short row, short col);
    void ResPlaceStone(short row, short col);
    void ResGameResult(STONE stone, int level, int myMoney);
    void ResChangePlayerLevel(int playerId, int level);
    void ResChangePlayerState(int playerId, PLAYER_STATE state);
    void ResEnterRoom(int playerId);
    void ReqLeaveRoom();
    void ResLeaveRoom(int playerId);
};
