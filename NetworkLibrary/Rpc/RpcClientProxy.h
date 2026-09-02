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
    void ResLobbyPlayers(std::vector<PlayerInfo>& players);
    void ResPlayerEnterLobby(PlayerInfo& player);
    void ResPlayerLeaveLobby(int playerId);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqBuyCharacter();
    void ResBuyCharacter(Character& character, int curGold);
    void ReqChangeEquipment(int inventoryId);
    void ResChangeEquipment(int inventoryId);
    void ReqStartGame();
    void ResStartGame(PlayerInfo& otherPlayer);
    void ReqCancelGame();
    void ReqGameCommand(int commandType);
    void ResGameCommand(Character& character);
    void ResEndGame(int result, int currentMoney);
};
