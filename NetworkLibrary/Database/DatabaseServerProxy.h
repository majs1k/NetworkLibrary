#pragma once

#include "../Utils/PacketQueue.h"
#include "../Utils/Packet.h"

class DatabaseServerProxy
{
public:
    PacketQueue* logicQueue_;
    PacketQueue* dbQueue_;

public:
    void ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password);
    void ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId);
    void ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName);
    void ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code);
    void ReqPlayerProfileDB(__int64 sessionId, int userId);
    void ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player);
    void ReqCharacterListDB(__int64 sessionId, int playerId);
    void ResCharacterListDB(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList);
    void ReqPlayerListDB(__int64 sessionId);
    void ResPlayerListDB(__int64 sessionId, std::list<Player> playerList);
    void ResPlayerEnterDB(__int64 sessionId, Player player);
    void ResPlayerLeaveDB(__int64 sessionId, int playerId);
    void ResPlayerDeleteDB(__int64 sessionId, int playerId);
    void ReqChatDB(__int64 sessionId, std::string& message);
    void ResChatDB(__int64 sessionId, int playerId, std::string& message);
    void ReqEnterRoomDB(__int64 sessionId, int roomId);
    void ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ReqUseItemDB(__int64 sessionId, std::list<int>& lst);
    void ResUseItemDB(__int64 sessionId, std::list<int>& lst);
};
