#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
    LanServer* server_;

public:
    void ReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    void ResRegister(__int64 sessionId, RESPONSE_CODE code);
    void ReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    void ResLogin(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName);
    void ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerProfile(__int64 sessionId, int userId);
    void ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player);
    void ReqRoomList(__int64 sessionId);
    void ResRoomList(__int64 sessionId);
    void ReqRoomCreate(__int64 sessionId, std::string& roomName);
    void ResRoomCreate(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName);
    void ResRoomDelete(__int64 sessionId, int roomId);
    void ReqEnterRoom(__int64 sessionId, int roomId);
    void ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ResEnterOtherUser(__int64 sessionId);
    void ReqPlayerList(__int64 sessionId);
    void ResPlayerList(__int64 sessionId, std::list<Player> playerList);
    void ReqChat(__int64 sessionId, std::string& message);
    void ResChat(__int64 sessionId, int playerId, std::string& message);
    void ReqUseItem(__int64 sessionId, std::list<int>& lst);
    void ResUseItem(__int64 sessionId, std::list<int>& lst);
};
