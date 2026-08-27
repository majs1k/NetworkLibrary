#pragma once

#include "../Utils/Packet.h"

class DatabaseServerHandler
{
public:
    virtual bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerEnterLobbyDB(__int64 sessionId, int userId);
    virtual bool ResPlayerProfileDB(__int64 sessionId, Player player);
    virtual bool ResPlayerCharactersDB(__int64 sessionId, std::list<Character> characterList);
    virtual bool ReqChatDB(__int64 sessionId, std::string& message);
    virtual bool ResChatDB(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqLobbyPlayersDB(__int64 sessionId);
    virtual bool ResLobbyPlayersDB(__int64 sessionId, std::list<Player> playerList);
    virtual bool ResPlayerEnterLobbyDB(__int64 sessionId, Player player);
    virtual bool ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId);
    virtual bool ReqBuyCharacterDB(__int64 sessionId);
    virtual bool ResBuyCharacterDB(__int64 sessionId, Character character, int currentMoney);
    virtual bool ReqEnterMatchQueueDB(__int64 sessionId);
    virtual bool ResEnterMatchQueueDB(__int64 sessionId, Player otherPlayer);
    virtual bool ResEndMatchDB(__int64 sessionId, int result, int currentMoney);
};

class DatabaseServerStub
{
public:
    DatabaseServerHandler* handler_;

public:
    bool DbPacketProc(__int64 sessionId, Packet* packet);
};
