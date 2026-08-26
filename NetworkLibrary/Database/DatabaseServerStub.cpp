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

            return handler_->ReqUserRegisterDB(sessionId, loginId, password);
        }
        case 1:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResUserRegisterDB(sessionId, code);
        }
        case 2:
        {
			std::string loginId;
			std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserLoginDB(sessionId, loginId, password);
        }
        case 3:
        {
			RESPONSE_CODE code;
			int userId;

            *packet >> code >> userId;

            return handler_->ResUserLoginDB(sessionId, code, userId);
        }
        case 10:
        {
			int userId;
			std::string playerName;

            *packet >> userId >> playerName;

            return handler_->ReqPlayerRegisterDB(sessionId, userId, playerName);
        }
        case 11:
        {
			RESPONSE_CODE code;

            *packet >> code;

            return handler_->ResPlayerRegisterDB(sessionId, code);
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
        case 14:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ReqCharacterListDB(sessionId, playerId);
        }
        case 15:
        {
			RESPONSE_CODE code;
			std::list<Character> characterList;

            *packet >> code >> characterList;

            return handler_->ResCharacterListDB(sessionId, code, characterList);
        }
        case 20:
        {

            *packet;

            return handler_->ReqPlayerListDB(sessionId);
        }
        case 21:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResPlayerListDB(sessionId, playerList);
        }
        case 22:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnterDB(sessionId, player);
        }
        case 23:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveDB(sessionId, playerId);
        }
        case 24:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerDeleteDB(sessionId, playerId);
        }
        case 30:
        {
			std::string message;

            *packet >> message;

            return handler_->ReqChatDB(sessionId, message);
        }
        case 31:
        {
			int playerId;
			std::string message;

            *packet >> playerId >> message;

            return handler_->ResChatDB(sessionId, playerId, message);
        }
        case 50:
        {
			int roomId;

            *packet >> roomId;

            return handler_->ReqEnterRoomDB(sessionId, roomId);
        }
        case 51:
        {
			RESPONSE_CODE code;
			int roomId;
			std::string roomName;
			std::list<int> lst;

            *packet >> code >> roomId >> roomName >> lst;

            return handler_->ResEnterRoomDB(sessionId, code, roomId, roomName, lst);
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


bool DatabaseServerHandler::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
    return true;
}

bool DatabaseServerHandler::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
    return true;
}

bool DatabaseServerHandler::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
    return true;
}

bool DatabaseServerHandler::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
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

bool DatabaseServerHandler::ReqCharacterListDB(__int64 sessionId, int playerId)
{
    return true;
}

bool DatabaseServerHandler::ResCharacterListDB(__int64 sessionId, RESPONSE_CODE code, std::list<Character> characterList)
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

bool DatabaseServerHandler::ResPlayerEnterDB(__int64 sessionId, Player player)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerLeaveDB(__int64 sessionId, int playerId)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerDeleteDB(__int64 sessionId, int playerId)
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

bool DatabaseServerHandler::ReqEnterRoomDB(__int64 sessionId, int roomId)
{
    return true;
}

bool DatabaseServerHandler::ResEnterRoomDB(__int64 sessionId, RESPONSE_CODE code, int roomId, std::string& roomName, std::list<int>& lst)
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
