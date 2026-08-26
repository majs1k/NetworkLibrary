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

            return handler_->ReqUserRegister(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResUserRegister(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserLogin(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResUserLogin(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqPlayerRegister(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResPlayerRegister(sessionId, code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return handler_->ReqPlayerProfile(sessionId, userId);
        }
        case 13:
        {
			RESPONSE_CODE code;
			Player player;

            *packet >> code >> player;

            return handler_->ResPlayerProfile(sessionId, code, player);
        }
        case 14:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ReqCharacterList(sessionId, playerId);
        }
        case 15:
        {
			RESPONSE_CODE code;
			std::list<Character> characterList;

            *packet >> code >> characterList;

            return handler_->ResCharacterList(sessionId, code, characterList);
        }
        case 20:
        {

            *packet;

            return handler_->ReqPlayerList(sessionId);
        }
        case 21:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerList(sessionId, playerList);
        }
        case 22:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnter(sessionId, player);
        }
        case 23:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeave(sessionId, playerId);
        }
        case 24:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerDelete(sessionId, playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChat(sessionId, message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChat(sessionId, playerId, message);
        }
        case 50:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoom(sessionId, roomId);
        }
        case 51:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoom(sessionId, code, roomId, roomName, lst);
        }
        case 200:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ReqUseItem(sessionId, lst);
        }
        case 201:
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


bool RpcServerHandler::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResUserRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool RpcServerHandler::ResUserLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcServerHandler::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool RpcServerHandler::ResPlayerRegister(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool RpcServerHandler::ReqPlayerProfile(__int64 sessionId, int userId)
{
    return true;
}

bool RpcServerHandler::ResPlayerProfile(__int64 sessionId, RESPONSE_CODE code, Player player)
{
    return true;
}

bool RpcServerHandler::ReqCharacterList(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerHandler::ResCharacterList(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList)
{
    return true;
}

bool RpcServerHandler::ReqPlayerList(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ResPlayerList(__int64 sessionId, std::list<Player> playerList)
{
    return true;
}

bool RpcServerHandler::ResPlayerEnter(__int64 sessionId, Player player)
{
    return true;
}

bool RpcServerHandler::ResPlayerLeave(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerHandler::ResPlayerDelete(__int64 sessionId, int playerId)
{
    return true;
}

bool RpcServerHandler::ReqChat(__int64 sessionId, std::string& message)
{
    return true;
}

bool RpcServerHandler::ResChat(__int64 sessionId, int playerId, std::string& message)
{
    return true;
}

bool RpcServerHandler::ReqEnterRoom(__int64 sessionId, int roomId)
{
    return true;
}

bool RpcServerHandler::ResEnterRoom(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
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
