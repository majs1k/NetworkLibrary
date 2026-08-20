#pragma once

#include "../Utils/PacketQueue.h"
#include "../Utils/Packet.h"

class DatabaseServerProxy
{
public:
    PacketQueue* queue_;

public:
    void DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password);
    void DbResRegister(__int64 sessionId, RESPONSE_CODE code);
    void DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password);
    void DbResLogin(__int64 sessionId, RESPONSE_CODE code);
    void DbReqChat(__int64 sessionId, std::string& chat);
    void DbResChat(__int64 sessionId, std::string& chat);
    void DbReqUseItem(__int64 sessionId, std::list<int>& lst);
    void DbResUseItem(__int64 sessionId, std::list<int>& lst);
};
