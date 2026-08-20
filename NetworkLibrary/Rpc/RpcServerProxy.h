#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
    LanServer* server_;

public:
    void ReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    void ResRegister(__int64 sessionId, RESPONSE_CODE code);
    void ReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    void ResLogin(__int64 sessionId, RESPONSE_CODE code);
    void ReqChat(__int64 sessionId, std::string& chat);
    void ResChat(__int64 sessionId, std::string& chat);
    void ReqUseItem(__int64 sessionId, std::list<int>& lst);
    void ResUseItem(__int64 sessionId, std::list<int>& lst);
};
