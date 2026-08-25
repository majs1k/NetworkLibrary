#pragma once

#include "../Utils/Packet.h"

class RpcServerHandler
{
public:
    virtual bool ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerProfile(__int64 sessionId, int userId);
    virtual bool ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player);
    virtual bool ReqPlayerList(__int64 sessionId);
    virtual bool ResPlayerList(__int64 sessionId, std::list<Player> playerList);
    virtual bool ResPlayerEnter(__int64 sessionId, Player player);
    virtual bool ResPlayerLeave(__int64 sessionId, int playerId);
    virtual bool ResPlayerDelete(__int64 sessionId, int playerId);
    virtual bool ReqChat(__int64 sessionId, std::string& message);
    virtual bool ResChat(__int64 sessionId, int playerId, std::string& message);
    virtual bool ReqEnterRoom(__int64 sessionId, int roomId);
    virtual bool ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    virtual bool ReqUseItem(__int64 sessionId, std::list<int>& lst);
    virtual bool ResUseItem(__int64 sessionId, std::list<int>& lst);
};

class RpcServerStub
{
public:
    RpcServerHandler* handler_;

public:
    bool PacketProc(__int64 sessionId, Packet* packet);
};
