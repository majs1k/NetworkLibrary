#pragma once

#include "../Utils/Packet.h"

class RpcClientStub
{
public:

    bool PacketProc(Packet* packet);
    
    virtual bool ReqUserRegister(std::string& loginId, std::string& password);
    virtual bool ResUserRegister(RESPONSE_CODE code);
    virtual bool ReqUserLogin(std::string& loginId, std::string& password);
    virtual bool ResUserLogin(RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegister(int userId, std::string& playerName);
    virtual bool ResPlayerRegister(RESPONSE_CODE code);
    virtual bool ReqPlayerConnection(int userId);
    virtual bool ResPlayerProfile(Player& player);
    virtual bool ResPlayerCharacters(std::list<Character>& characters);
    virtual bool ResLobbyPlayers(std::list<PlayerInfo>& players);
    virtual bool ResPlayerEnterLobby(PlayerInfo& player);
    virtual bool ResPlayerLeaveLobby(int playerId);
    virtual bool ReqChat(std::string& message);
    virtual bool ResChat(int playerId, std::string& message);
    virtual bool ReqBuyCharacter();
    virtual bool ResBuyCharacter(Character& character, int curGold);
    virtual bool ReqChangeEquipment(int inventoryId);
    virtual bool ResChangeEquipment(int inventoryId);
    virtual bool ReqStartGame();
    virtual bool ResStartGame(PlayerInfo& otherPlayer);
    virtual bool ReqGameCommand(int commandType);
    virtual bool ResGameCommand(Character& character);
    virtual bool ResEndGame(int result, int currentMoney);
};
