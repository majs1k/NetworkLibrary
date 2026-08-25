#pragma once

#include "../Network/LanClient.h"
#include "../Utils/Packet.h"

class RpcClientProxy
{
public:
    LanClient* client_;

public:
    void ReqUserRegister(std::string& loginId, std::string& password);
    void ResUserRegister(RESPONSE_CODE code);
    void ReqUserLogin(std::string& loginId, std::string& password);
    void ResUserLogin(RESPONSE_CODE code, int userId);
    void ReqCreatePlayer(int userId, std::string& playerName);
    void ResCreatePlayer(RESPONSE_CODE code);
    void ReqPlayerProfile(int userId);
    void ResPlayerProfile(RESPONSE_CODE code, Player player);
    void ReqPlayerList();
    void ResPlayerList(std::list<Player> playerList);
    void ResPlayerEnter(Player player);
    void ResPlayerLeave(int playerId);
    void ResPlayerDelete(int playerId);
    void ReqChat(std::string& message);
    void ResChat(int playerId, std::string& message);
    void ReqEnterRoom(int roomId);
    void ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    void ReqUseItem(std::list<int>& lst);
    void ResUseItem(std::list<int>& lst);
};
