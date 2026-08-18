#pragma once

#include "../Utils/Packet.h"

class RpcClientStub
{
public:
    bool PacketProc(Packet* packet);

public:
    virtual bool LoginRequest(int userId);
    virtual bool LoginResponse(int userId);
    virtual bool ChatRequest(std::string& chat);
    virtual bool ChatResponse(std::string& chat);
    virtual bool ItemRequest(std::list<int>& lst);
    virtual bool ItemResponse(std::list<int>& lst);
};
