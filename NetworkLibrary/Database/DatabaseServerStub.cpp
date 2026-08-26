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

            return handler_->ReqPlayerEnterLobbyDB(sessionId, userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerProfileDB(sessionId, player);
        }
        case 14:
        {
			std::list<Character> characterList;

            *packet >> characterList;

            return handler_->ResPlayerCharactersDB(sessionId, characterList);
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
        case 40:
        {

            *packet;

            return handler_->ReqLobbyPlayersDB(sessionId);
        }
        case 41:
        {
			std::list<Player> playerList;

            *packet >> playerList;

            return handler_->ResLobbyPlayersDB(sessionId, playerList);
        }
        case 42:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerEnterLobbyDB(sessionId, player);
        }
        case 43:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveLobbyDB(sessionId, playerId);
        }
        case 50:
        {

            *packet;

            return handler_->ReqBuyCharacterDB(sessionId);
        }
        case 51:
        {
			Character character;
			int currentMoney;

            *packet >> character >> currentMoney;

            return handler_->ResBuyCharacterDB(sessionId, character, currentMoney);
        }
        case 60:
        {

            *packet;

            return handler_->ReqEnterMatchDB(sessionId);
        }
        case 61:
        {

            *packet;

            return handler_->ResEnterMatchDB(sessionId);
        }
        case 62:
        {

            *packet;

            return handler_->ResStartMatchDB(sessionId);
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

bool DatabaseServerHandler::ReqPlayerEnterLobbyDB(__int64 sessionId, int userId)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerProfileDB(__int64 sessionId, Player player)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerCharactersDB(__int64 sessionId, std::list<Character> characterList)
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

bool DatabaseServerHandler::ReqLobbyPlayersDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResLobbyPlayersDB(__int64 sessionId, std::list<Player> playerList)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerEnterLobbyDB(__int64 sessionId, Player player)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
{
    return true;
}

bool DatabaseServerHandler::ReqBuyCharacterDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResBuyCharacterDB(__int64 sessionId, Character character, int currentMoney)
{
    return true;
}

bool DatabaseServerHandler::ReqEnterMatchDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResEnterMatchDB(__int64 sessionId)
{
    return true;
}

bool DatabaseServerHandler::ResStartMatchDB(__int64 sessionId)
{
    return true;
}
