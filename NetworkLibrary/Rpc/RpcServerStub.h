#pragma once

#include "../Utils/Packet.h"

class RpcServerHandler
{
public:
    virtual bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerEnterLobby(__int64 sessionId, int userId);
    virtual bool ResPlayerProfile(__int64 sessionId, Player player);
    virtual bool ResPlayerCharacters(__int64 sessionId, std::list<Character> characterList);
    virtual bool ReqChat(__int64 sessionId, std::string& message);
    virtual bool ResChat(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqLobbyPlayers(__int64 sessionId);
    virtual bool ResLobbyPlayers(__int64 sessionId, std::list<Player> playerList);
    virtual bool ResPlayerEnterLobby(__int64 sessionId, Player player);
    virtual bool ResPlayerLeaveLobby(__int64 sessionId, int playerId);
    virtual bool ReqBuyCharacter(__int64 sessionId);
    virtual bool ResBuyCharacter(__int64 sessionId, Character character, int currentMoney);
    virtual bool ReqEnterMatch(__int64 sessionId);
    virtual bool ResEnterMatch(__int64 sessionId);
    virtual bool ResStartMatch(__int64 sessionId);
};

class RpcServerStub
{
public:
    RpcServerHandler* handler_;

public:
    bool PacketProc(__int64 sessionId, Packet* packet);
};
