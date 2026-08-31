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

            return handler_->ReqPlayerConnectionDB(sessionId, userId);
        }
        case 13:
        {
			Player player;

            *packet >> player;

            return handler_->ResPlayerProfileDB(sessionId, player);
        }
        case 14:
        {
			std::list<Character> characters;

            *packet >> characters;

            return handler_->ResPlayerCharactersDB(sessionId, characters);
        }
        case 15:
        {
			std::list<PlayerInfo> players;

            *packet >> players;

            return handler_->ResLobbyPlayersDB(sessionId, players);
        }
        case 16:
        {
			PlayerInfo player;

            *packet >> player;

            return handler_->ResPlayerEnterLobbyDB(sessionId, player);
        }
        case 17:
        {
			int playerId;

            *packet >> playerId;

            return handler_->ResPlayerLeaveLobbyDB(sessionId, playerId);
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
			int playerId;
			int inventoryId;
			int characterId;
			int curGold;

            *packet >> playerId >> inventoryId >> characterId >> curGold;

            return handler_->ReqBuyCharacterDB(sessionId, playerId, inventoryId, characterId, curGold);
        }
        case 51:
        {
			Character character;
			int curGold;

            *packet >> character >> curGold;

            return handler_->ResBuyCharacterDB(sessionId, character, curGold);
        }
        case 52:
        {
			int playerId;
			int inventoryId;

            *packet >> playerId >> inventoryId;

            return handler_->ReqChangeEquipmentDB(sessionId, playerId, inventoryId);
        }
        case 64:
        {
			int result;
			int currentMoney;

            *packet >> result >> currentMoney;

            return handler_->ReqEndGameDB(sessionId, result, currentMoney);
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

bool DatabaseServerHandler::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerCharactersDB(__int64 sessionId, std::list<Character>& characters)
{
    return true;
}

bool DatabaseServerHandler::ResLobbyPlayersDB(__int64 sessionId, std::list<PlayerInfo>& players)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerEnterLobbyDB(__int64 sessionId, PlayerInfo& player)
{
    return true;
}

bool DatabaseServerHandler::ResPlayerLeaveLobbyDB(__int64 sessionId, int playerId)
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

bool DatabaseServerHandler::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold)
{
    return true;
}

bool DatabaseServerHandler::ResBuyCharacterDB(__int64 sessionId, Character& character, int curGold)
{
    return true;
}

bool DatabaseServerHandler::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
    return true;
}

bool DatabaseServerHandler::ReqEndGameDB(__int64 sessionId, int result, int currentMoney)
{
    return true;
}
