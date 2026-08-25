#pragma once

#include "../Utils/Packet.h"

class RpcClientHandler
{
public:
    virtual bool ReqUserRegister(std::string& loginId, std::string& password);
    virtual bool ResUserRegister(RESPONSE_CODE code);
    virtual bool ReqUserLogin(std::string& loginId, std::string& password);
    virtual bool ResUserLogin(RESPONSE_CODE code, int userId);
    virtual bool ReqCreatePlayer(int userId, std::string& playerName);
    virtual bool ResCreatePlayer(RESPONSE_CODE code);
    virtual bool ReqPlayerProfile(int userId);
    virtual bool ResPlayerProfile(RESPONSE_CODE code, Player player);
    virtual bool ReqPlayerList();
    virtual bool ResPlayerList(std::list<Player> playerList);
    virtual bool ResPlayerEnter(Player player);
    virtual bool ResPlayerLeave(int playerId);
    virtual bool ResPlayerDelete(int playerId);
    virtual bool ReqChat(std::string& message);
    virtual bool ResChat(int playerId, std::string& message);
    virtual bool ReqEnterRoom(int roomId);
    virtual bool ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    virtual bool ReqUseItem(std::list<int>& lst);
    virtual bool ResUseItem(std::list<int>& lst);
};

class RpcClientStub
{
public:
    RpcClientHandler* handler_;

public:
    bool PacketProc(Packet* packet);
};
