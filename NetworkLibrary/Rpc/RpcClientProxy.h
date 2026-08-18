#pragma once

#include "../Network/LanClient.h"
#include "../Utils/Packet.h"

class RpcClientProxy
{
public:
    LanClient* client_;

public:
    void LoginRequest(int userId);
    void LoginResponse(int userId);
    void ChatRequest(std::string& chat);
    void ChatResponse(std::string& chat);
    void ItemRequest(std::list<int>& lst);
    void ItemResponse(std::list<int>& lst);
};
