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
    void ReqPlayerEnterLobby(int userId);
    void ResPlayerProfile(Player player);
    void ResPlayerCharacters(std::list<Character> characterList);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqLobbyPlayers();
    void ResLobbyPlayers(std::list<Player> playerList);
    void ResPlayerEnterLobby(Player player);
    void ResPlayerLeaveLobby(int playerId);
    void ReqBuyCharacter();
    void ResBuyCharacter(Character character, int currentMoney);
    void ReqEnterMatchQueue();
    void ResEnterMatchQueue(Player otherPlayer);
    void ResEndMatch(int result, int currentMoney);
};
