#include "DatabaseServerStub.h"


bool DatabaseServerStub::DbPacketProc(__int64 sessionId, Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->DbReqRegister(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->DbResRegister(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->DbReqLogin(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->DbResLogin(sessionId, code);
        }
        case 10:
        {
			std::string chat;

            *packet >> chat;

            return handler_->DbReqChat(sessionId, chat);
        }
        case 11:
        {
			std::string chat;

            *packet >> chat;

            return handler_->DbResChat(sessionId, chat);
        }
        case 30:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->DbReqUseItem(sessionId, lst);
        }
        case 31:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->DbResUseItem(sessionId, lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool DatabaseServerHandler::DbReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::DbResRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DatabaseServerHandler::DbReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::DbResLogin(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DatabaseServerHandler::DbReqChat(__int64 sessionId, std::string& chat)
{
    return true;
}

bool DatabaseServerHandler::DbResChat(__int64 sessionId, std::string& chat)
{
    return true;
}

bool DatabaseServerHandler::DbReqUseItem(__int64 sessionId, std::list<int>& lst)
{
    return true;
}

bool DatabaseServerHandler::DbResUseItem(__int64 sessionId, std::list<int>& lst)
{
    return true;
}
