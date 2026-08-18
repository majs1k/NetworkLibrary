#pragma once

#include "../Utils/Packet.h"

class RpcServerStub
{
public:
    bool PacketProc(__int64 sessionId, Packet* packet);

public:
    virtual bool LoginRequest(__int64 sessionId, int userId);
    virtual bool LoginResponse(__int64 sessionId, int userId);
    virtual bool ChatRequest(__int64 sessionId, std::string& chat);
    virtual bool ChatResponse(__int64 sessionId, std::string& chat);
    virtual bool ItemRequest(__int64 sessionId, std::list<int>& lst);
    virtual bool ItemResponse(__int64 sessionId, std::list<int>& lst);
};
