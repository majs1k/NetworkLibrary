#include "DatabaseServerProxy.h"


void DatabaseServerProxy::DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbResRegister(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 1;
    *packet << code;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 2;
    *packet << loginId << password;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbResLogin(__int64 sessionId, RESPONSE_CODE code)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 3;
    *packet << code;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbReqChat(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 10;
    *packet << chat;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbResChat(__int64 sessionId, std::string& chat)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 11;
    *packet << chat;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbReqUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 30;
    *packet << lst;

    queue_->Push(packet);
}

void DatabaseServerProxy::DbResUseItem(__int64 sessionId, std::list<int>& lst)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 31;
    *packet << lst;

    queue_->Push(packet);
}
