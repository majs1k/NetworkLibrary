#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
    LanServer* server_;

public:
    void ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserRegister(__int64 sessionId, RESPONSE_CODE code);
    void ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName);
    void ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerProfile(__int64 sessionId, int userId);
    void ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player);
    void ReqPlayerList(__int64 sessionId);
    void ResPlayerList(__int64 sessionId, std::list<Player> playerList);
    void ResPlayerEnter(__int64 sessionId, Player player);
    void ResPlayerLeave(__int64 sessionId, int playerId);
    void ResPlayerDelete(__int64 sessionId, int playerId);
    void ReqChat(__int64 sessionId, std::string& message);
    void ResChat(__int64 sessionId, int playerId, std::string& message);
    void ReqEnterRoom(__int64 sessionId, int roomId);
    void ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ReqUseItem(__int64 sessionId, std::list<int>& lst);
    void ResUseItem(__int64 sessionId, std::list<int>& lst);
};
