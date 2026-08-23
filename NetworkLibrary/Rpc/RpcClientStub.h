#pragma once

#include "../Utils/Packet.h"

class RpcClientHandler
{
public:
    virtual bool ReqRegister(std::string& loginId, std::string& password);
    virtual bool ResRegister(RESPONSE_CODE code);
    virtual bool ReqLogin(std::string& loginId, std::string& password);
    virtual bool ResLogin(RESPONSE_CODE code, int userId);
    virtual bool ReqCreatePlayer(int userId, std::string& playerName);
    virtual bool ResCreatePlayer(RESPONSE_CODE code);
    virtual bool ReqPlayerProfile(int userId);
    virtual bool ResPlayerProfile(RESPONSE_CODE code, Player player);
    virtual bool ReqRoomList();
    virtual bool ResRoomList();
    virtual bool ReqRoomCreate(std::string& roomName);
    virtual bool ResRoomCreate(RESPONSE_CODE code, int roomId, std::string& roomName);
    virtual bool ResRoomDelete(int roomId);
    virtual bool ReqEnterRoom(int roomId);
    virtual bool ResEnterRoom(RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst);
    virtual bool ResEnterOtherUser();
    virtual bool ReqPlayerList();
    virtual bool ResPlayerList(std::list<Player> playerList);
    virtual bool ReqChat(std::string& message);
    virtual bool ResChat(int playerId, std::string& message);
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
