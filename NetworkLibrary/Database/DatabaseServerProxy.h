#pragma once

#include "../Utils/PacketQueue.h"
#include "../Utils/Packet.h"

class DatabaseServerProxy
{
public:
    PacketQueue* logicQueue_;
    PacketQueue* dbQueue_;

public:
    void ReqRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqCreatePlayerDB(__int64 sessionId, int userId, std::string& playerName);
    void ResCreatePlayerDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerProfileDB(__int64 sessionId, int userId);
    void ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player);
    void ReqRoomListDB(__int64 sessionId);
    void ResRoomListDB(__int64 sessionId);
    void ReqRoomCreateDB(__int64 sessionId, std::string& roomName);
    void ResRoomCreateDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName);
    void ResRoomDeleteDB(__int64 sessionId, int roomId);
    void ReqEnterRoomDB(__int64 sessionId, int roomId);
    void ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ResEnterOtherUserDB(__int64 sessionId);
    void ReqPlayerListDB(__int64 sessionId);
    void ResPlayerListDB(__int64 sessionId, std::list<Player> playerList);
    void ReqChatDB(__int64 sessionId, std::string& message);
    void ResChatDB(__int64 sessionId, int playerId, std::string& message);
    void ReqUseItemDB(__int64 sessionId, std::list<int>& lst);
    void ResUseItemDB(__int64 sessionId, std::list<int>& lst);
};
