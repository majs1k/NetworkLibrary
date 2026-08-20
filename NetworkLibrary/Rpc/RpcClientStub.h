#pragma once

#include "../Utils/Packet.h"

class RpcClientHandler
{
public:
    virtual bool ReqRegister(std::string& loginId, std::string& password);
    virtual bool ResRegister(RESPONSE_CODE code);
    virtual bool ReqLogin(std::string& loginId, std::string& password);
    virtual bool ResLogin(RESPONSE_CODE code);
    virtual bool ReqChat(std::string& chat);
    virtual bool ResChat(std::string& chat);
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
