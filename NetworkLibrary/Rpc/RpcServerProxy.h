#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
    LanServer* server_;

public:
    void LoginRequest(__int64 sessionId, int userId);
    void LoginResponse(__int64 sessionId, int userId);
    void ChatRequest(__int64 sessionId, std::string& chat);
    void ChatResponse(__int64 sessionId, std::string& chat);
    void ItemRequest(__int64 sessionId, std::list<int>& lst);
    void ItemResponse(__int64 sessionId, std::list<int>& lst);
};
