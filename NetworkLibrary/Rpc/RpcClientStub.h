#pragma once

#include "../Utils/Packet.h"

class RpcClientHandler
{
public:
    virtual bool ReqUserRegister(std::string& loginId, std::string& password);
    virtual bool ResUserRegister(RESPONSE_CODE code);
    virtual bool ReqUserLogin(std::string& loginId, std::string& password);
    virtual bool ResUserLogin(RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegister(int userId, std::string& playerName);
    virtual bool ResPlayerRegister(RESPONSE_CODE code);
    virtual bool ReqPlayerEnterLobby(int userId);
    virtual bool ResPlayerProfile(Player& player);
    virtual bool ResPlayerCharacters(std::list<Character>& characters);
    virtual bool ResLobbyPlayers(std::list<PlayerInfo>& players);
    virtual bool ResPlayerEnterLobby(PlayerInfo& player);
    virtual bool ResPlayerLeaveLobby(int playerId);
    virtual bool ReqChat(std::string& message);
    virtual bool ResChat(int playerId, std::string& message);
    virtual bool ReqBuyCharacter();
    virtual bool ResBuyCharacter(Character& character, int currentMoney);
    virtual bool ReqEnterMatchQueue();
    virtual bool ResEnterMatchQueue(PlayerInfo& otherPlayer);
    virtual bool ResEndMatch(int result, int currentMoney);
};

class RpcClientStub
{
public:
    RpcClientHandler* handler_;

public:
    bool PacketProc(Packet* packet);
};
