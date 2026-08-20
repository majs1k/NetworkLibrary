#include "RpcClientStub.h"


bool RpcClientStub::PacketProc(Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqRegister(loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResRegister(code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqLogin(loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResLogin(code);
        }
        case 10:
        {
			std::string chat;

            *packet >> chat;

            return handler_->ReqChat(chat);
        }
        case 11:
        {
			std::string chat;

            *packet >> chat;

            return handler_->ResChat(chat);
        }
        case 30:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ReqUseItem(lst);
        }
        case 31:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ResUseItem(lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool RpcClientHandler::ReqRegister(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResRegister(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientHandler::ReqLogin(std::string& loginId, std::string& password)
{
    return true;
}

bool RpcClientHandler::ResLogin(RESPONSE_CODE code)
{
    return true;
}

bool RpcClientHandler::ReqChat(std::string& chat)
{
    return true;
}

bool RpcClientHandler::ResChat(std::string& chat)
{
    return true;
}

bool RpcClientHandler::ReqUseItem(std::list<int>& lst)
{
    return true;
}

bool RpcClientHandler::ResUseItem(std::list<int>& lst)
{
    return true;
}
