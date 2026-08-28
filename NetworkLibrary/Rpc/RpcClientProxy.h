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
    void ResPlayerProfile(Player& player);
    void ResPlayerCharacters(std::list<Character>& characters);
    void ResLobbyPlayers(std::list<PlayerInfo>& players);
    void ResPlayerEnterLobby(PlayerInfo& player);
    void ResPlayerLeaveLobby(int playerId);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqBuyCharacter();
    void ResBuyCharacter(Character& character, int currentMoney);
    void ReqEnterMatchQueue();
    void ResEnterMatchQueue(PlayerInfo& otherPlayer);
    void ResEndMatch(int result, int currentMoney);
};
