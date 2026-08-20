#pragma once

#include "../Utils/Packet.h"

class RpcServerHandler
{
public:
    virtual bool ReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool ResLogin(__int64 sessionId, RESPONSE_CODE code);
    virtual bool ReqChat(__int64 sessionId, std::string& chat);
    virtual bool ResChat(__int64 sessionId, std::string& chat);
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
