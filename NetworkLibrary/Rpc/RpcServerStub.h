#pragma once

#include "../Utils/Packet.h"

class RpcServerHandler
{
public:
    virtual bool ReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    virtual bool ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName);
    virtual bool ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqPlayerProfile(__int64 sessionId, int userId);
    virtual bool ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player);
    virtual bool ReqRoomList(__int64 sessionId);
    virtual bool ResRoomList(__int64 sessionId);
    virtual bool ReqRoomCreate(__int64 sessionId, std::string& roomName);
    virtual bool ResRoomCreate(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName);
    virtual bool ResRoomDelete(__int64 sessionId, int roomId);
    virtual bool ReqEnterRoom(__int64 sessionId, int roomId);
    virtual bool ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    virtual bool ResEnterOtherUser(__int64 sessionId);
    virtual bool ReqPlayerList(__int64 sessionId);
    virtual bool ResPlayerList(__int64 sessionId, std::list<Player> playerList);
    virtual bool ReqChat(__int64 sessionId, std::string& message);
    virtual bool ResChat(__int64 sessionId, int playerId, std::string& message);
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
