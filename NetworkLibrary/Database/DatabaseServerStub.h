#pragma once

#include "../Utils/Packet.h"

class DatabaseServerHandler
{
public:
    virtual bool DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool DbResRegister(__int64 sessionId, RESPONSE_CODE code);
    virtual bool DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    virtual bool DbResLogin(__int64 sessionId, RESPONSE_CODE code);
    virtual bool DbReqChat(__int64 sessionId, std::string& chat);
    virtual bool DbResChat(__int64 sessionId, std::string& chat);
    virtual bool DbReqUseItem(__int64 sessionId, std::list<int>& lst);
    virtual bool DbResUseItem(__int64 sessionId, std::list<int>& lst);
};

class DatabaseServerStub
{
public:
    DatabaseServerHandler* handler_;

public:
    bool DbPacketProc(__int64 sessionId, Packet* packet);
};
