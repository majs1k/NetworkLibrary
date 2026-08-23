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

            return handler_->ReqRegisterDB(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResRegisterDB(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqLoginDB(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResLoginDB(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqCreatePlayerDB(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResCreatePlayerDB(sessionId, code);
        }
        case 12:
        {
			int userId;

            *packet >> userId;

            return handler_->ReqPlayerProfileDB(sessionId, userId);
        }
        case 13:
        {
			RESPONSE_CODE code;
			Player player;

            *packet >> code >> player;

            return handler_->ResPlayerProfileDB(sessionId, code, player);
        }
        case 20:
        {

            *packet;

            return handler_->ReqRoomListDB(sessionId);
        }
        case 21:
        {

            *packet;

            return handler_->ResRoomListDB(sessionId);
        }
        case 22:
        {
			std::string roomName;

            *packet >> roomName;

            return handler_->ReqRoomCreateDB(sessionId, roomName);
        }
        case 23:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;

            *packet >> code >> roomId >> roomName;

            return handler_->ResRoomCreateDB(sessionId, code, roomId, roomName);
        }
        case 24:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ResRoomDeleteDB(sessionId, roomId);
        }
        case 25:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoomDB(sessionId, roomId);
        }
        case 26:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoomDB(sessionId, code, roomId, roomName, lst);
        }
        case 27:
        {

            *packet;

            return handler_->ResEnterOtherUserDB(sessionId);
        }
        case 90:
        {

            *packet;

            return handler_->ReqPlayerListDB(sessionId);
        }
        case 91:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerListDB(sessionId, playerList);
        }
        case 100:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChatDB(sessionId, message);
        }
        case 101:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChatDB(sessionId, playerId, message);
        }
        case 200:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ReqUseItemDB(sessionId, lst);
        }
        case 201:
        {
			std::list<int> lst;

            *packet >> lst;

            return handler_->ResUseItemDB(sessionId, lst);
        }
    default:
    {
        // 로그 추가

        return false;
    }
    }
}


bool DatabaseServerHandler::ReqRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::ResRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DatabaseServerHandler::ReqLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::ResLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool DatabaseServerHandler::ReqCreatePlayerDB(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool DatabaseServerHandler::ResCreatePlayerDB(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DatabaseServerHandler::ReqPlayerProfileDB(__int64 sessionId, int userId)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerProfileDB(__int64 sessionId, RESPONSE_CODE code, Player player)
{
    return true;
}

bool DatabaseServerHandler::ReqRoomListDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResRoomListDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ReqRoomCreateDB(__int64 sessionId, std::string& roomName)
{
    return true;
}

bool DatabaseServerHandler::ResRoomCreateDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName)
{
    return true;
}

bool DatabaseServerHandler::ResRoomDeleteDB(__int64 sessionId, int roomId)
{
    return true;
}

bool DatabaseServerHandler::ReqEnterRoomDB(__int64 sessionId, int roomId)
{
    return true;
}

bool DatabaseServerHandler::ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
{
    return true;
}

bool DatabaseServerHandler::ResEnterOtherUserDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ReqPlayerListDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerListDB(__int64 sessionId, std::list<Player> playerList)
{
    return true;
}

bool DatabaseServerHandler::ReqChatDB(__int64 sessionId, std::string& message)
{
    return true;
}

bool DatabaseServerHandler::ResChatDB(__int64 sessionId, int playerId, std::string& message)
{
    return true;
}

bool DatabaseServerHandler::ReqUseItemDB(__int64 sessionId, std::list<int>& lst)
{
    return true;
}

bool DatabaseServerHandler::ResUseItemDB(__int64 sessionId, std::list<int>& lst)
{
    return true;
}
