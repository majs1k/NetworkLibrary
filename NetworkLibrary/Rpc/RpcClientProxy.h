#pragma once

#include "../Network/LanClient.h"
#include "../Utils/Packet.h"

class RpcClientProxy
{
public:
    LanClient* client_;

public:
    void ReqRegister(std::string& loginId, std::string& password);
    void ResRegister(RESPONSE_CODE code);
    void ReqLogin(std::string& loginId, std::string& password);
    void ResLogin(RESPONSE_CODE code, int userId);
    void ReqCreatePlayer(int userId, std::string& playerName);
    void ResCreatePlayer(RESPONSE_CODE code);
    void ReqPlayerProfile(int userId);
    void ResPlayerProfile(RESPONSE_CODE code, Player player);
    void ReqRoomList();
    void ResRoomList();
    void ReqRoomCreate(std::string& roomName);
    void ResRoomCreate(RESPONSE_CODE code, int roomId, std::string& roomName);
    void ResRoomDelete(int roomId);
    void ReqEnterRoom(int roomId);
    void ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ResEnterOtherUser();
    void ReqPlayerList();
    void ResPlayerList(std::list<Player> playerList);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqUseItem(std::list<int>& lst);
    void ResUseItem(std::list<int>& lst);
};
