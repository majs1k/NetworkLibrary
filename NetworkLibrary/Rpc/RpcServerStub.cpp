#include "RpcServerStub.h"


bool RpcServerStub::PacketProc(__int64 sessionId, Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqRegister(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResRegister(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqLogin(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResLogin(sessionId, code);
        }
        case 10:
        {
			std::string chat;

            *packet >> chat;

            return handler_->ReqChat(sessionId, chat);
        }
        case 11:
        {
			std::string chat;

            *packet >> chat;

            return handler_->ResChat(sessionId, chat);
        }
        case 30:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ReqUseItem(sessionId, lst);
        }
        case 31:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ResUseItem(sessionId, lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcServerHandler::ReqRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResLogin(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqChat(__int64 sessionId, std::string& chat)
{
    return true;
}

bool RpcServerHandler::ResChat(__int64 sessionId, std::string& chat)
{
    return true;
}

bool RpcServerHandler::ReqUseItem(__int64 sessionId, std::list<int>& lst)
{
    return true;
}

bool RpcServerHandler::ResUseItem(__int64 sessionId, std::list<int>& lst)
{
    return true;
}
