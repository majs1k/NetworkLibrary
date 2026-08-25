#pragma once

#include "../Utils/Packet.h"

class DatabaseServerHandler
{
public:
    virtual bool ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqCreatePlayerDB(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResCreatePlayerDB(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerProfileDB(__int64 sessionId, int userId);
    virtual bool ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player);
    virtual bool ReqPlayerListDB(__int64 sessionId);
    virtual bool ResPlayerListDB(__int64 sessionId, std::list<Player> playerList);
    virtual bool ResPlayerEnterDB(__int64 sessionId, Player player);
    virtual bool ResPlayerLeaveDB(__int64 sessionId, int playerId);
    virtual bool ResPlayerDeleteDB(__int64 sessionId, int playerId);
    virtual bool ReqChatDB(__int64 sessionId, std::string& message);
    virtual bool ResChatDB(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqEnterRoomDB(__int64 sessionId, int roomId);
    virtual bool ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    virtual bool ReqUseItemDB(__int64 sessionId, std::list<int>& lst);
    virtual bool ResUseItemDB(__int64 sessionId, std::list<int>& lst);
};

class DatabaseServerStub
{
public:
    DatabaseServerHandler* handler_;

public:
    bool DbPacketProc(__int64 sessionId, Packet* packet);
};
