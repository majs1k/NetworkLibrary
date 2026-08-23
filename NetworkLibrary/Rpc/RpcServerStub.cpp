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
			int userId;

            *packet >> code >> userId;

            return handler_->ResLogin(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqCreatePlayer(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResCreatePlayer(sessionId, code);
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
        case 20:
        {

            *packet;

            return handler_->ReqRoomList(sessionId);
        }
        case 21:
        {

            *packet;

            return handler_->ResRoomList(sessionId);
        }
        case 22:
        {
			std::string roomName;

            *packet >> roomName;

            return handler_->ReqRoomCreate(sessionId, roomName);
        }
        case 23:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;

            *packet >> code >> roomId >> roomName;

            return handler_->ResRoomCreate(sessionId, code, roomId, roomName);
        }
        case 24:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ResRoomDelete(sessionId, roomId);
        }
        case 25:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoom(sessionId, roomId);
        }
        case 26:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoom(sessionId, code, roomId, roomName, lst);
        }
        case 27:
        {

            *packet;

            return handler_->ResEnterOtherUser(sessionId);
        }
        case 90:
        {

            *packet;

            return handler_->ReqPlayerList(sessionId);
        }
        case 91:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerList(sessionId, playerList);
        }
        case 100:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChat(sessionId, message);
        }
        case 101:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChat(sessionId, playerId, message);
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

bool RpcServerHandler::ResLogin(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool RpcServerHandler::ReqCreatePlayer(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool RpcServerHandler::ResCreatePlayer(__int64 sessionId, RESPONSE_CODE code)
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

bool RpcServerHandler::ReqRoomList(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ResRoomList(__int64 sessionId)
{
    return true;
}

bool RpcServerHandler::ReqRoomCreate(__int64 sessionId, std::string& roomName)
{
    return true;
}

bool RpcServerHandler::ResRoomCreate(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName)
{
    return true;
}

bool RpcServerHandler::ResRoomDelete(__int64 sessionId, int roomId)
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

bool RpcServerHandler::ResEnterOtherUser(__int64 sessionId)
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

bool RpcServerHandler::ReqChat(__int64 sessionId, std::string& message)
{
    return true;
}

bool RpcServerHandler::ResChat(__int64 sessionId, int playerId, std::string& message)
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
