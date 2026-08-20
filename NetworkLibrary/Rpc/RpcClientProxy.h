#pragma once

#include "../Network/LanClient.h"
#include "../Utils/Packet.h"

class RpcClientProxy
{
public:
    LanClient* client_;

public:
    void ReqRegister(std::string& loginId, std::string& password);
    void ResRegister(RESPONSE_CODE code);
    void ReqLogin(std::string& loginId, std::string& password);
    void ResLogin(RESPONSE_CODE code);
    void ReqChat(std::string& chat);
    void ResChat(std::string& chat);
    void ReqUseItem(std::list<int>& lst);
    void ResUseItem(std::list<int>& lst);
};
